#pragma once
#include <QObject>
#include <QtSql>
#include <QDateTime>
#include <QAbstractListModel>

#include "../internal/api_structs.h"
#include "database.h"

namespace timetable {
class LocalStorage : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void save(TableType type,const QVariant& data) {
        using namespace internal;
        decltype (auto) instance = Database::instance();
        if (data.canConvert<Timetable>()) {
            Timetable timetable = qvariant_cast<Timetable>(data);
            instance.Save(type,timetable);
        }
        else if (data.canConvert<SavedTimetable>()) {
            SavedTimetable timetable = qvariant_cast<SavedTimetable>(data);
            instance.Save(type,timetable);
        }
    }
    Q_INVOKABLE void transactionSave(TableType type,const QVariantList& data) {
        using namespace internal;
        decltype (auto) instance = Database::instance();
        instance.beginTransaction();
        for (const QVariant& item : data)
            save(type,item);
        instance.commit();
    }
    Q_INVOKABLE QVariantList get(TableType type) {
        using namespace internal;

        QVariantList data;
        QEventLoop waiter;
        auto callback = [&](const QVariant& items) {
            data = qvariant_cast<QVariantList>(items);
            waiter.quit();
        };
        decltype (auto) instance = Database::instance();
        switch (type) {
        case TableType::SEARCH_TEACHER:
        case TableType::SEARCH_GROUP:
            instance.Get<Timetable>(type,callback);
            break;
        case TableType::SAVED_TIMETABLE:
            instance.Get<SavedTimetable>(type,callback);
            break;
        default:
            break;
        }
        waiter.exec();
        return data;
    }
    Q_INVOKABLE void delete_(TableType type,int id) {
        Database::instance().Delete(type,id);
    }
    Q_INVOKABLE QVariant createTimetable(int id,QString title,bool isTeacher,const QVariantList& lessons) const {
        using namespace internal;
        SavedTimetable timetable;
        timetable.id = id;
        timetable.title = title;
        timetable.isTeacher = isTeacher;
        timetable.lastUpdate = QDateTime::currentDateTime().toString();
        timetable.lessons.reserve(static_cast<uint>(lessons.size()));
        for (const auto& it : lessons) {
            timetable.lessons.push_back(qvariant_cast<Lesson>(it));
        }
        return QVariant::fromValue(timetable);
    }
    Q_INVOKABLE void beginTransaction() {
        Database::instance().beginTransaction();
    }
    Q_INVOKABLE void endTransaction() {
        Database::instance().commit();
    }
    Q_INVOKABLE void clearStorage() {
        Database::instance().clearDatabase();
    }
signals:
    void error(const QString& message);
    void success(const QString& message);
};
}
