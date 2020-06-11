#pragma once
#include <QObject>
#include <QtSql>
#include <QDateTime>
#include <QAbstractListModel>
#include "../internal/api_structs.h"
namespace timetable {
class LocalStorage : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void saveTimetable(int id,const QString& title,
                                   const QVariantList& lessons) {
        internal::SavedTimetable timetable;
        timetable.id = id;
        timetable.title = title;
        timetable.lastUpdate = QDateTime::currentDateTime().toString();
        timetable.lessons.reserve(lessons.size());
        for (const QVariant& lesson : lessons)
            timetable.lessons.push_back(qvariant_cast<internal::Lesson>(lesson));
        emit saveTimetable(QVariant::fromValue(timetable));
    }
    Q_INVOKABLE void deleteTimetable(int id) {

    }
signals:
    void saveTimetable(const QVariant& timetable);
};
}
