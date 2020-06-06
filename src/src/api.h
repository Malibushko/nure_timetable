#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "internal/api_base.h"

namespace timetable {
class ApiJSON : public internal::api::ApiBase {

    QString API_ROOT;
    QNetworkAccessManager* mng;
public:
    ApiJSON(QObject * obj = nullptr) : internal::api::ApiBase(obj),mng{new QNetworkAccessManager(this)} {};
    ApiJSON(const QString& root ,QObject* obj = nullptr) : internal::api::ApiBase(obj),
                                                           API_ROOT{root},
                                                           mng{new QNetworkAccessManager(this)} {
        using namespace internal;
        QObject::connect(this,&ApiJSON::facultiesResponse,[](const QList<Faculty>& list){
            for (auto & it : list)
                qDebug() << it.id << it.full_name << it.short_name;
        });
        QObject::connect(this,&ApiJSON::error,[](const QString& list){
            qDebug() << "[ERROR]:" << list;
        });
    }
    
    QString getRoot() const {
        return API_ROOT;
    }
    void setRoot(const QString& root) {
        API_ROOT = root;
    }

    void faculties() override;
    void teachers(int32_t p_id_department) override;
    void departments(int32_t p_id_faculty) override;
    void specialities(int32_t p_id_faculty, int32_t p_id_department) override;
    void schedule(int32_t p_group_id) override;
    void directions(int32_t p_id_faculty) override;
};

}
