#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "internal/api_base.h"

namespace timetable {
/**
 * @brief The ApiJSON class
 *
 * Class provides access from NURE JSON api
 */
class ApiJSON : public internal::api::ApiBase {
    Q_OBJECT
private:
    static const inline QString API_ROOT{"https://cist.nure.ua/ias/app/tt/"}; /**< Root for all requests */
    QNetworkAccessManager* m_mng;

    /**< Two variables that represents some magical numbers for API  (see comment in constructor) */
    QString m_dateStart;
    QString m_dateEnd;
public:
    ApiJSON() : m_mng{new QNetworkAccessManager(this)} {
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
            m_dateStart = QString("01.01." + QString::number(QDate::currentDate().year()));
            m_dateEnd = QString("31.07."+ QString::number(QDate::currentDate().year()));
        } else {
            m_dateStart = QString("01.09." + QString::number(QDate::currentDate().year()));
            m_dateEnd = QString("31.01."+ QString::number(QDate::currentDate().year()+1));
        }
    };
    /**
     * @brief Get API root
     * @return QString that contain API root
     */
    Q_INVOKABLE QString getRoot() const;
    /**
     * @brief Synchronous request for timetable
     * @param Timetable id
     * @param Boolean flag that tells if timetable is for teacher of group
     * @return QVariantList of Lessons
     */
    Q_INVOKABLE QVariantList scheduleSync(int id,bool isTeacher);
    /**
     * @brief Asynchronous request for timetable
     * @param Timetable id
     * @param Boolean flag that tells if timetable is for teacher of group
     * @return emit newLesson signal while processing
     */
    Q_INVOKABLE void schedule(int id,bool isTeacher);

    /// Request for list of groups (see ApiBase)
    Q_INVOKABLE void groups() override;
    /// Request for list of teachers (see ApiBase)
    Q_INVOKABLE void teachers() override;
signals:
    /**
     * @brief Signal is emited when a lesson was parsed
       from CSV and ready to be added to model
     * @param Timetable id
     * @param Lesson object
     */
    void newLesson(int id,const QVariant& lesson);
    /**
     * @brief Signal is emited when first lesson processed
     * \note can be used for animations
     */
    void transferingFinished();
    /**
     * @brief Signal is emitted after last lesson processed
     * \note can be used for animations
     */
    void transferingStarted();
    /**
     * @brief Signal is emited when new timetable arrived
     * @param Number of rows in new timetable
     */
    void timetableAboutToBeArrived(int rowCount);
};

}
