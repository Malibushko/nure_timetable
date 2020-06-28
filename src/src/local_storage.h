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
    Q_INVOKABLE void save(TableType type,const QVariant& data);
    Q_INVOKABLE void transactionSave(TableType type,const QVariantList& data);
    Q_INVOKABLE QVariantList get(TableType type);
    Q_INVOKABLE void delete_(TableType type,int id);
    Q_INVOKABLE QVariant createTimetable(int id,QString title,bool isTeacher,const QVariantList& lessons) const;
    Q_INVOKABLE void beginTransaction();
    Q_INVOKABLE void endTransaction();
    Q_INVOKABLE void clearStorage();
signals:
    void error(const QString& message);
    void success(const QString& message);
};

}
