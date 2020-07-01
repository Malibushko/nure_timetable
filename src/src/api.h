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
    static const inline QString API_ROOT{"http://cist.nure.ua/ias/app/tt/"};
    QNetworkAccessManager* mng;
    QString dateStart;
    QString dateEnd;
public:
    ApiJSON() : mng{new QNetworkAccessManager(this)} {
        /**
          This part looks really strange, doesn`t it? It looks like that to me too.
          NURE hid this (it had 3 API`s last year) API 6 month ago and there is no documentation
          about this part at all.
          I found this source in my previous app and copy-paste it there, because it is the
          only way to get timetable for group/teacher now. This dates ("01.01", "01.08", etc)
          are magical constants, that have never been documented in API (I found out them myself)
          so I prefer not to change this, in order not to broke this and lost the only API
          I have.
          */
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

    Q_INVOKABLE QVariantList scheduleSync(int id,bool isTeacher);
    Q_INVOKABLE void schedule(int id,bool isTeacher);

    Q_INVOKABLE void groups() override;
    Q_INVOKABLE void teachers() override;
signals:
    void newLesson(int id,const QVariant& lesson);
    void transferingFinished();
    void transferingStarted();
    void timetableAboutToBeArrived(int rowCount);
};

}
