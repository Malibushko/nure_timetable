#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "internal/api_base.h"

namespace timetable {
class ApiJSON : public QObject, public internal::api::ApiBase {
    Q_OBJECT
    QString API_ROOT;
    QNetworkAccessManager* mng;
public:
    ApiJSON(QObject * obj = nullptr) : QObject{obj},mng{new QNetworkAccessManager(this)} {};
    ApiJSON(const QString& root, QObject* obj = nullptr) : QObject{obj}, 
                                                           API_ROOT{root},
                                                           mng{new QNetworkAccessManager(this)} {}
    
    QString getRoot() const {
        return API_ROOT;
    }
    void setRoot() {
        
    }
    void faculties() override;
    void teachers(int32_t p_id_department) override;
    void departments(int32_t p_id_faculty) override;
    void specialities(int32_t p_id_faculty, int32_t p_id_department) override;
    void schedule(int32_t p_group_id) override;
    void directions(int32_t p_id_faculty) override;
};

}
