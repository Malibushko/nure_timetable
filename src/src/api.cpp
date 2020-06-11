#include <QUrlQuery>
#include <QTextCodec>
#include <QTextDecoder>
#include "api.h"
#include "internal/json_parser.h"

namespace timetable {
namespace internal::traits {

template<typename T>
struct is_container<QList<T>> : std::true_type {};
}

inline namespace details {
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
void ApiJSON::setRoot(const QString& root) {
    API_ROOT = root;
}

void ApiJSON::faculties() {
    QUrl request(API_ROOT+map(API_TYPES::P_API_FACULTIES_JSON));
    QNetworkReply * r = mng->get(QNetworkRequest{request});
    connect(mng,&QNetworkAccessManager::finished,[=](QNetworkReply*){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::UniversityWrapper response = internal::JsonParser::i().
                fromJson<internal::UniversityWrapper>(details::decode1251(r->readAll().data()).data());
        emit facultiesResponse(response.university.faculties);
    });
};
void  ApiJSON::teachers(int p_id_department) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_TEACHERS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_department",QString::number(p_id_department));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }

        internal::DepartmentWrapper department = internal::JsonParser::i().
                fromJson<internal::DepartmentWrapper>(details::decode1251(r->readAll().data()).data());
        emit teachersResponse(department.department.teachers);
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::departments(int p_id_faculty) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_DEPARTMENTS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::FacultyWrapper faculty = internal::JsonParser::i().
                fromJson<internal::FacultyWrapper>(details::decode1251(r->readAll().data()).data());

        emit departmentsResponse(faculty.faculty.departments);
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::specialities(int p_id_faculty, int p_id_direction) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_SPECIALITIES_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    q.addQueryItem("p_id_direction",QString::number(p_id_direction));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this,p_id_direction](QNetworkReply* r){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::FacultyWrapper faculty = internal::JsonParser::i().
                fromJson<internal::FacultyWrapper>(details::decode1251(r->readAll().data()).data());
        for (auto & it : faculty.faculty.directions)
            if (it.id == p_id_direction) {
                emit specialitiesResponse(it.specialities);
                break;
            }
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::directions(int p_id_faculty) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_DIRECTIONS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::FacultyWrapper faculty = internal::JsonParser::i().
                fromJson<internal::FacultyWrapper>(details::decode1251(r->readAll().data()).data());

        emit directionsResponse(faculty.faculty.directions);
    });
    mng->get(QNetworkRequest{request});
}

void ApiJSON::schedule(int id,bool isTeacher) {
        QUrl getUrl(API_ROOT+ (isTeacher ? map(API_TYPES::P_API_TEACHER_REQUEST)
                                      : map(API_TYPES::P_API_GROUP_REQUEST)));
        QUrlQuery query;
        query.addQueryItem("ATypeDoc","3");
        query.addQueryItem((isTeacher ? "Aid_sotr":"Aid_group"),QString::number(id));
        query.addQueryItem((isTeacher ? "Aid_kaf" : "Aid_potok"),"0");
        query.addQueryItem("ADateStart",dateStart);
        query.addQueryItem("ADateEnd",dateEnd);
        getUrl.setQuery(query);

        QNetworkReply * r = mng->get(QNetworkRequest{getUrl});
        connect(r,&QNetworkReply::finished,[=](){
           if (r->error() != QNetworkReply::NoError) {
               qDebug() << "Error in " << __func__ << " :" << r->error();
               return;
           }
           QStringList rows = QString(decode1251(r->readAll())).split('\r',Qt::SkipEmptyParts);
           // remove header
           if (rows.empty()) {
               qDebug() << "No data";
               return;
           }
           rows.pop_front();
           emit timetableAboutToBeArrived(rows.size());
           for (const QString& row : rows) {
                emit newLesson(id,QVariant::fromValue(internal::Lesson::fromCSV(row)));
           }
        });
}

void ApiJSON::groups() {
    emit cacheFind(Database::TableType::SEARCH_GROUP);

    bool exitFlag = false;
    QEventLoop loop;
    connect(&cache,&Cache::cacheHit,[&](bool b){
        if (!b) {
            qDebug() << "Cache miss";
        }
        else {
            qDebug() << "Cache hit";
            exitFlag = true;
            emit cacheGet(Database::TableType::SEARCH_GROUP);
        }
    });
    connect(&cache,&Cache::cacheHit,&loop,&QEventLoop::quit);
    loop.exec();
    if (exitFlag)
        return;

    qDebug() << "Network Request";
    QUrl request(API_ROOT+map(API_TYPES::P_API_GROUP_JSON));
    QNetworkReply * r = mng->get(QNetworkRequest{request});
    connect(r,&QNetworkReply::finished,[=](){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::UniversityWrapper response = internal::JsonParser::i().
                fromJson<internal::UniversityWrapper>(details::decode1251(r->readAll().data()).data());
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
    });
}
void ApiJSON::teachers() {

    emit cacheFind(Database::TableType::SEARCH_TEACHER);

    bool exitFlag = false;
    QEventLoop loop;
    connect(&cache,&Cache::cacheHit,[&](bool b){
        if (!b) {
            qDebug() << "Cache miss teacher";
        }
        else {
            qDebug() << "Cache hit teacher";
            exitFlag = true;
            emit cacheGet(Database::TableType::SEARCH_TEACHER);
        }
    });
    connect(&cache,&Cache::cacheHit,&loop,&QEventLoop::quit);
    loop.exec();
    if (exitFlag)
        return;

    QUrl request(API_ROOT+map(API_TYPES::P_API_PODR_JSON));
    QNetworkReply * r = mng->get(QNetworkRequest{request});
    connect(r,&QNetworkReply::finished,[=](){
        if (r->error() != QNetworkReply::NoError) {
            emit error(r->errorString());
            return;
        }
        internal::UniversityWrapper response = internal::JsonParser::i().
                fromJson<internal::UniversityWrapper>(details::decode1251(r->readAll().data()).data());
        for (const auto & faculty : response.university.faculties) {
            for (const auto & department : faculty.departments) {
                for (const auto & teacher : department.teachers)
                    emit teacherResponse(QVariant::fromValue(internal::Timetable{teacher.id,teacher.short_name}));
            }
        }
    });
}
}
