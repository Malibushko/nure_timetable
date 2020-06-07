#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "internal/api_base.h"

namespace timetable {

class ApiJSON : public internal::api::ApiBase {
    Q_OBJECT
private:
    QString API_ROOT{"http://cist.nure.ua/ias/app/tt/"};
    QNetworkAccessManager* mng;
public:
    ApiJSON() : mng{new QNetworkAccessManager(this)} {};
    ApiJSON(const QString& root) : API_ROOT{root}, mng{new QNetworkAccessManager(this)} {}

    Q_INVOKABLE QString getRoot() const;
    Q_INVOKABLE void    setRoot(const QString& root);

    Q_INVOKABLE void faculties() override;
    Q_INVOKABLE void groups() override;
    Q_INVOKABLE void teachers() override;
    Q_INVOKABLE void teachers(int32_t p_id_department) override;
    Q_INVOKABLE void departments(int32_t p_id_faculty) override;
    Q_INVOKABLE void specialities(int32_t p_id_faculty, int32_t p_id_department) override;
    Q_INVOKABLE void schedule(int32_t p_group_id) override;
    Q_INVOKABLE void directions(int32_t p_id_faculty) override;
};

}
