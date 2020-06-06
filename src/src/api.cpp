#include <QUrlQuery>

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
        P_API_AUDITORY_TYPES_JSON
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
        }
    }
}
void ApiJSON::faculties() {
    QUrl request(API_ROOT+map(API_TYPES::P_API_FACULTIES_JSON));

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit facultiesResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Faculty>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
};
void  ApiJSON::teachers(int32_t p_id_department) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_TEACHERS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_department",QString::number(p_id_department));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit teachersResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Teacher>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::departments(int32_t p_id_faculty) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_DEPARTMENTS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit departmentsResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Department>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::specialities(int32_t p_id_faculty, int32_t p_id_department) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_SPECIALITIES_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    q.addQueryItem("p_id_department",QString::number(p_id_department));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit specialitiesResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Speciality>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
}
void ApiJSON::directions(int32_t p_id_faculty) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_DIRECTIONS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_id_faculty));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit directionsResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Direction>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
}

void ApiJSON::schedule(int32_t p_group_id) {
    QUrl request(API_ROOT+map(API_TYPES::P_API_DIRECTIONS_JSON));
    QUrlQuery q;
    q.addQueryItem("p_id_faculty",QString::number(p_group_id));
    request.setQuery(q);

    connect(mng,&QNetworkAccessManager::finished,[this](QNetworkReply* r){
       if (r->error() != QNetworkReply::NoError) {
           emit error(r->errorString());
           return;
       }
       emit directionsResponse(internal::JsonParser::i()
                              .fromJson<QList<internal::Direction>>(r->readAll().data()));
    });
    mng->get(QNetworkRequest{request});
}

}
