#pragma once
#include <QObject>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QtSql>
#include <iostream>
#include "../internal/executor.h"
#include "../internal/query_builder.h"
#include "../internal/json_parser.h"
namespace timetable {

using DefaultCallback = std::function<void(QVariant)>;

class Database : public QObject{
    Q_OBJECT
private:
    QString tableName;
    ExecutorService se;
private:
    explicit Database(QObject * obj = nullptr) : QObject{obj} {
        QEventLoop waiter;
        se.submit([](){
            return QVariant::fromValue(Database::connect());
        }).then([&](QVariant table){
            tableName = qvariant_cast<QString>(table);
            waiter.quit();
        });
        waiter.exec();
    }
    static QString connect() {
        // create dir with information
        QDir dir{QStandardPaths::writableLocation(QStandardPaths::DataLocation)};
        auto connection = QSqlDatabase::addDatabase("QSQLITE");
        connection.setDatabaseName(dir.path() + QDir::separator() + "storage");
        if (!connection.open()) {
            qDebug() << "Unable to create database;\n Error: \"" << connection.lastError().text()
                     << "\"";
            return "";
        } else {
            qDebug() << dir.path();
            if (!connection.tables().size())
                QSqlQuery q("CREATE TABLE`kvalue` ("
                            "`id`	INTEGER NOT NULL,"
                            "`type`	INTEGER NOT NULL,"
                            "`data`	TEXT,"
                            "PRIMARY KEY(`id`)"
                            ");");
            qDebug() << "Database opened; \nTables:" << connection.tables();
            return connection.tables().first();
        }
    }
public:
    enum class TableType : uint8_t {
        SETTINGS        = 1 << 0,
        SEARCH_TEACHER  = 1 << 1,
        SEARCH_GROUP    = 1 << 2,
        SAVED_REF       = 1 << 3,
        SAVED_TIMETABLE = 1 << 4
    };
    Q_ENUM(TableType);
    static Database& instance() {
        static Database db;
        return db;
    }
    template<class Resource,class Callback = DefaultCallback>
    void Save(TableType type,const Resource& r,Callback&& cb = {}) {
        using namespace internal;
        QueryBuilder builder;
        builder.replace().table(tableName)
                .values({QString::number(r.id),QString::number(static_cast<int>(type)),
                         '\''+QString(JsonParser::i().toJson(r).data())+'\''});
        se.submit([query = builder.query()](){
            QSqlQuery q(query);
            qDebug() << query;
            return QVariant::fromValue(q.lastInsertId());
        }).then(cb);
    }
    template<class Resource,class Callback = DefaultCallback>
    void Get(TableType type,Callback&& cb = {}) {
        using namespace  internal;
        QueryBuilder builder;
        builder.select().table(tableName)
                .where(QString("type = %1").arg(static_cast<int>(type)));
        se.submit([query = builder.query()](){
            QSqlQuery q(query);
            QVariantList result;
            while (q.next()) {
                result.push_back(QVariant::fromValue(
                                     JsonParser::i().fromJson<Resource>(q.value("data").
                                                                        toString().toStdString().data())));
            }
            return QVariant::fromValue(result);
        }).then(cb);
    }

    template<class Resource,class Callback = DefaultCallback>
    void Get(TableType type, const QString& condition,Callback&& cb = {}) {
        using namespace internal;
        QueryBuilder builder;
        builder.select().table(tableName).where(QString("type = %1").arg(static_cast<int>(type)))
                .and_(condition);

        se.submit([query = builder.query()](){
            QSqlQuery q(query);
            QVariantList result;
            while (q.next()) {
                result.push_back(QVariant::fromValue(
                                     JsonParser::i().fromJson<Resource>(q.value("data").
                                                                        toString().toStdString().data())));
            }
            return QVariant::fromValue(result);
        }).then(cb);
    }
    template<class Callback = DefaultCallback>
    void Delete(TableType type, int id,Callback&& cb = {}) {
        using namespace internal;
        QueryBuilder builder;
        builder.delete_().table(tableName);
        if (id != -1)
            builder.where(QString("id = %1").arg(id)).and_(QString("type = %1").arg(static_cast<int>(type)));
        se.submit([query = builder.query()]{
            QSqlQuery q(query);
            qDebug() << query;
            return QVariant::fromValue(q.lastError().type() != QSqlError::NoError);
        }).then(cb);
    }

signals:
    emit void unableToConnect();
    emit void error(QString description);
};
}
