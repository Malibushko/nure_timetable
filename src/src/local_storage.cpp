#include "local_storage.h"

namespace timetable {
void LocalStorage::save(TableType type, const QVariant &data) {
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


void LocalStorage::transactionSave(TableType type, const QVariantList &data) {
    using namespace internal;
    decltype (auto) instance = Database::instance();
    instance.beginTransaction();
    for (const QVariant& item : data)
        save(type,item);
    instance.commit();
}


QVariantList LocalStorage::get(TableType type) {
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


void LocalStorage::delete_(TableType type, int id) {
    Database::instance().Delete(type,id);
}


QVariant LocalStorage::createTimetable(int id, QString title, bool isTeacher, const QVariantList &lessons) const {
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


void LocalStorage::beginTransaction() {
    Database::instance().beginTransaction();
}


void LocalStorage::endTransaction() {
    Database::instance().commit();
}


void LocalStorage::clearStorage() {
    Database::instance().clearDatabase();
}

}
