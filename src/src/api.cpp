#include <QUrlQuery>
#include <QTextCodec>
#include <QTextDecoder>
#include <QEventLoop>
#include "api.h"
#include "internal/json_parser.h"

namespace timetable {
namespace internal::traits {

template<typename T>
struct is_container<QList<T>> : std::true_type {};
}

inline namespace details {
    // Api tokens and mapping

    enum class API_TYPES {
        P_API_PODR_JSON,
        P_API_GROUP_JSON,
        P_API_AUDITORIES_JSON,
        P_API_FACULTIES_JSON,
        P_API_DEPARTMENTS_JSON,
        P_API_TEACHERS_JSON,
        P_API_DIRECTIONS_JSON,
        P_API_SPECIALITIES_JSON,
        P_API_GRP_OF_DIRECTIONS_JSON,
        P_API_GRP_OF_SPECIALITIES_JSON,
        P_API_AUDITORY_TYPES_JSON,
        P_API_TEACHER_REQUEST,
        P_API_GROUP_REQUEST
    };
    QString map(API_TYPES value) {
        switch (value) {
        case API_TYPES::P_API_PODR_JSON:
            return "P_API_PODR_JSON";
        case API_TYPES::P_API_GROUP_JSON:
            return "P_API_GROUP_JSON";
        case API_TYPES::P_API_AUDITORIES_JSON:
            return "P_API_AUDITORIES_JSON";
        case API_TYPES::P_API_FACULTIES_JSON:
            return "P_API_FACULTIES_JSON";
        case API_TYPES::P_API_DEPARTMENTS_JSON:
            return "P_API_DEPARTMENTS_JSON";
        case API_TYPES::P_API_TEACHERS_JSON:
            return "P_API_TEACHERS_JSON";
        case API_TYPES::P_API_DIRECTIONS_JSON:
            return "P_API_DIRECTIONS_JSON";
        case API_TYPES::P_API_SPECIALITIES_JSON:
            return "P_API_SPECIALITIES_JSON";
        case API_TYPES::P_API_GRP_OF_DIRECTIONS_JSON:
            return "P_API_GRP_OF_DIRECTIONS_JSON";
        case API_TYPES::P_API_GRP_OF_SPECIALITIES_JSON:
            return "P_API_GRP_OF_SPECIALITIES_JSON";
        case API_TYPES::P_API_AUDITORY_TYPES_JSON:
            return "P_API_AUDITORY_TYPES_JSON";
        case API_TYPES::P_API_TEACHER_REQUEST:
            return "WEB_IAS_TT_GNR_RASP.GEN_TEACHER_KAF_RASP";
        case API_TYPES::P_API_GROUP_REQUEST:
            return "WEB_IAS_TT_GNR_RASP.GEN_GROUP_POTOK_RASP";
        }
       return {};
    }
    QByteArray decode1251(const char* data) {
        QTextCodec * codec = QTextCodec::codecForName("WINDOWS-1251");
        QTextDecoder * decoder = codec->makeDecoder();
        QByteArray encodedData(decoder->toUnicode(data).toUtf8());
        return encodedData;
    }
}
QString ApiJSON::getRoot() const {
    return API_ROOT;
}
void ApiJSON::schedule(int id,bool isTeacher) {
    QUrl getUrl(API_ROOT+ (isTeacher ? map(API_TYPES::P_API_TEACHER_REQUEST)
                                     : map(API_TYPES::P_API_GROUP_REQUEST)));
    /*
     * Form a request
     * Note: Api was lost (NURE API refused to give a new one)
     */
    QUrlQuery query;
    query.addQueryItem("ATypeDoc","3");
    query.addQueryItem((isTeacher ? "Aid_sotr":"Aid_group"),QString::number(id));
    query.addQueryItem((isTeacher ? "Aid_kaf" : "Aid_potok"),"0");
    query.addQueryItem("ADateStart",m_dateStart);
    query.addQueryItem("ADateEnd",m_dateEnd);
    getUrl.setQuery(query);

    QNetworkReply * r = m_mng->get(QNetworkRequest{getUrl});
    connect(r,&QNetworkReply::finished,[=](){
        if (r->error() != QNetworkReply::NoError) {
            QString errMsg = tr("Error occured\nProbably you have been banned by cist.nure.ua because you"
                                  " made too many requests today. Try later");
            emit error(errMsg);
            return;
        }
        QStringList rows = QString(decode1251(r->readAll())).split('\r',Qt::SkipEmptyParts);
        // remove header
        if (rows.size() <= 1) {
            qDebug() << "No data";
            emit error(tr("This timetable does not contain any information. Try later or "
                       "ask API maintainers about your timetable."));
            return;
        }
        rows.pop_front();
        emit timetableAboutToBeArrived(rows.size());
        for (const QString& row : rows) {
            emit newLesson(id,QVariant::fromValue(internal::Lesson::fromCSV(row)));
        }
    });
}
QVariantList ApiJSON::scheduleSync(int id,bool isTeacher) {
    QUrl getUrl(API_ROOT+ (isTeacher ? map(API_TYPES::P_API_TEACHER_REQUEST)
                                     : map(API_TYPES::P_API_GROUP_REQUEST)));
    QUrlQuery query;
    query.addQueryItem("ATypeDoc","3");
    query.addQueryItem((isTeacher ? "Aid_sotr":"Aid_group"),QString::number(id));
    query.addQueryItem((isTeacher ? "Aid_kaf" : "Aid_potok"),"0");
    query.addQueryItem("ADateStart",m_dateStart);
    query.addQueryItem("ADateEnd",m_dateEnd);
    getUrl.setQuery(query);

    QEventLoop waiter;
    QNetworkReply * r = m_mng->get(QNetworkRequest{getUrl});
    connect(r,&QNetworkReply::finished,&waiter,&QEventLoop::quit);
    waiter.exec();

    if (r->error() != QNetworkReply::NoError) {
        QString errMsg = tr("Error occured\nProbably you have been banned by cist.nure.ua because you"
                              " made too many requests today. Try later");
        emit error(errMsg);
        return {};
    }
    QStringList rows = QString(decode1251(r->readAll())).split('\r',Qt::SkipEmptyParts);
    if (rows.size() <= 1) {
        emit error(tr("This timetable does not contain any information. Try later or "
                   "ask API maintainers about your timetable."));
        return {};
    }
    // remove header
    rows.pop_front();
    QVariantList result;
    result.reserve(rows.size());

    for (const QString& row : rows) {
        result.push_back(QVariant::fromValue(internal::Lesson::fromCSV(row)));
    }
    return result;
}

void ApiJSON::groups() {
    QUrl request(API_ROOT+map(API_TYPES::P_API_GROUP_JSON));
    QNetworkReply * r = m_mng->get(QNetworkRequest{request});
    connect(r,&QNetworkReply::finished,[=](){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::UniversityWrapper response = internal::JsonParser::i().
                fromJson<internal::UniversityWrapper>(details::decode1251(r->readAll().data()).data());
        emit transferingStarted();
        for (const auto & faculty : response.university.faculties) {
            for (const auto & direction : faculty.directions) {
                for (const auto & group : direction.groups) {
                    emit groupResponse(QVariant::fromValue(internal::Timetable{group.id,group.name}));
                }
                for (const auto & speciality : direction.specialities) {
                    for (const auto & group : speciality.groups) {
                        emit groupResponse(QVariant::fromValue(internal::Timetable{group.id,group.name}));
                    }
                }
            }
        }
        emit transferingFinished();
    });
}
void ApiJSON::teachers() {
    QUrl request(API_ROOT+map(API_TYPES::P_API_PODR_JSON));
    QNetworkReply * r = m_mng->get(QNetworkRequest{request});
    connect(r,&QNetworkReply::finished,[=](){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::UniversityWrapper response = internal::JsonParser::i().
                fromJson<internal::UniversityWrapper>(details::decode1251(r->readAll().data()).data());
        emit transferingStarted();
        for (const auto & faculty : response.university.faculties) {
            for (const auto & department : faculty.departments) {
                for (const auto & teacher : department.teachers)
                    emit teacherResponse(QVariant::fromValue(internal::Timetable{teacher.id,teacher.short_name}));
            }
        }

        emit transferingFinished();
    });
}
}
