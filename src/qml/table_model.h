#pragma once
#include <QObject>
#include <QAbstractTableModel>
#include <QDebug>
#include <algorithm>
#include <set>
#include "../internal/api_structs.h"
namespace timetable {
/**
 * @brief The TableModel class
 *
 * Represents model for displaying Table
 */
class TableModel : public QAbstractTableModel {
    Q_OBJECT
    QString m_title; /**< Title of current timetable */
    int m_timetableId = -1; /**< Id of current timetable */

    QList<QDateTime>       m_horizontalHeaderData; /**< Horizontal header data (Dates) */
    QList<QTime> m_verticalHeaderData; /**< Vertical header data (Time of lessons) */

    std::pair<QTime,int> m_currentLesson; /**< Current lesson (used for caching) */
    QHash<qint64,internal::Lesson> m_lessons; /**< Container of lessons */

    int m_totalRows; /**< Total number of rows in timetable */

    enum TableRoles {
        DATE = Qt::UserRole,
        SUBJECT,
        TYPE,
        GROUPS,
        AUDITORY,
        TIME_START,
        TIME_END
    };
private:
    void generateVerticalHeader();
    void generateHorizontalHeader();
public:
    TableModel();

    /**
     * @brief Get id of current timetable
     * @return integer representing the id or -1 if not timetable was set
     */
    Q_INVOKABLE int id() const noexcept;
    /**
     * @brief Get duration of current lesson
     * @return integer representing the duration of lesson in seconds
     */
    Q_INVOKABLE int lessonDuration() const;
    /**
     * @brief Get progress (e.g how much time passed since start of lesson) of row
     * @param row number
     * @return integer representing lesson progress in seconds
     */
    Q_INVOKABLE int rowProgress(int row);
    /**
     * @brief Add new lesson to model
     * @param Lesson id
     * @param Lesson object
     */
    Q_INVOKABLE void addLesson(int id,const QVariant& lesson);
    /**
     * @brief Clear timetable and reserve space
     * @param Number of rows in new timetable
     */
    Q_INVOKABLE void prepareForNewTimetable(int rowCount);
    /**
     * @brief Clear timetable and headers
     */
    Q_INVOKABLE void clear();
    /**
     * @brief Get number of current columns
     * \note Current column is column that represents the current day
       (horizontal header is just list of dates)
     * @return intereger that represents current column
     */
    Q_INVOKABLE int currentColumn();
    /**
     * @brief Get raw model data
     * @return QVariantList of Lessons
     */
    Q_INVOKABLE QVariantList getModel() const;
    /**
     * @brief Get horizontal header
     * @return QStringList of dates
     */
    Q_INVOKABLE QList<QDateTime> horizontalHeaderData() const;
    /**
     * @brief Get vertical header
     * @return QStringList of times
     */
    Q_INVOKABLE QList<QTime>  verticalHeaderData() const;
    /**
     * @brief Get string representation of time in seconds
     * @param number of seconds
     * @return String representing time
     *
     * E.g if we pass 6300 the result will be 1:45:00
     */
    Q_INVOKABLE QString secondsToString(uint seconds);

    // override methods
    int rowCount(const QModelIndex& = {}) const override;
    int columnCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
signals:
    /**
     * @brief Signal is emited when last lesson appears to model
     */
    void timetableCompleted();
    /**
     * @brief Signal is emited when horizontal header generation is completed
     */
    void horizontalHeaderFinished();
    /**
     * @brief Signal is emited when vertical header generation is completed
     */
    void verticalHeaderFinished();
};

}
Q_DECLARE_METATYPE(QList<QTime>)
Q_DECLARE_METATYPE(QList<QDateTime>)
