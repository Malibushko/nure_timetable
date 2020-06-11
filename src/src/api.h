#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "internal/api_base.h"
#include "cache.h"

namespace timetable {

class ApiJSON : public internal::api::ApiBase {
    Q_OBJECT
private:
    QString API_ROOT{"http://cist.nure.ua/ias/app/tt/"};
    QNetworkAccessManager* mng;
    QThread cacheThread;
    Cache cache;
    QString dateStart;
    QString dateEnd;
public:
    ApiJSON() : mng{new QNetworkAccessManager(this)} {

        connect(this,&ApiJSON::groupResponse,&cache,&Cache::SaveSlotGroup);
        connect(this,&ApiJSON::teacherResponse,&cache,&Cache::SaveSlotTeacher);
        connect(this,&ApiJSON::cacheFind,&cache,&Cache::cacheCheck);
        connect(this,&ApiJSON::cacheGet,&cache,&Cache::cacheGet);
        connect(&cache,&Cache::cacheData,[&](const QVariant& v,Database::TableType type){
            switch (type) {
                case Database::TableType::SEARCH_GROUP:
                    emit groupResponse(v);
                    break;
                case Database::TableType::SEARCH_TEACHER:
                    emit teacherResponse(v);
                    break;
                default:
                    break;
            }
        });
        cache.moveToThread(&cacheThread);
        cacheThread.start();

        if (QDate::currentDate().month() < 8 ) {
            dateStart = QString("01.01." + QString::number(QDate::currentDate().year()));
            dateEnd = QString("31.07."+ QString::number(QDate::currentDate().year()));
        } else {
            dateStart = QString("01.08." + QString::number(QDate::currentDate().year()));
            dateEnd = QString("01.01."+ QString::number(QDate::currentDate().year()+1));
        }
    };
    ApiJSON(const QString& root) : ApiJSON{} {
        API_ROOT = root;
    }
    Q_INVOKABLE QString getRoot() const;
    Q_INVOKABLE void    setRoot(const QString& root);

    Q_INVOKABLE void schedule(int id,bool isTeacher);

    Q_INVOKABLE void faculties() override;
    Q_INVOKABLE void groups() override;
    Q_INVOKABLE void teachers() override;
    Q_INVOKABLE void teachers(int p_id_department) override;
    Q_INVOKABLE void departments(int p_id_faculty) override;
    Q_INVOKABLE void specialities(int p_id_faculty, int p_id_department) override;
    Q_INVOKABLE void directions(int p_id_faculty) override;
signals:
    void cacheFind(Database::TableType type);
    void cacheGet(Database::TableType type);
    void newLesson(int id,const QVariant& lesson);
    void timetableAboutToBeArrived(int rowCount);
};

}
