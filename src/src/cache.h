#pragma once
#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QtSql/QtSql>
#include "../internal/api_structs.h"
#include "../internal/json_parser.h"

namespace  timetable {
class Cache : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase connection;
public:
    enum class CacheType : uint8_t {
        SETTINGS        = 1 << 0,
        SEARCH_TEACHER  = 1 << 1,
        SEARCH_GROUP    = 1 << 2,
        SAVED_REF       = 1 << 3,
        SAVED_TIMETABLE = 1 << 4
    };
    Q_ENUM(CacheType);

    QString resourcePath = "";
    QList<QSqlQuery> insertQueue;
    QTimer* insertTimer;


    Cache(QObject *parent = nullptr) : QObject{parent}
    {
    }
public slots:
    bool tryConnect() {
        static const char * folder_name = "timetable_data";
        // create dir with information
        QDir dir{QStandardPaths::writableLocation(QStandardPaths::DataLocation)};
        dir.cd(folder_name);
        if (!dir.exists())
            if(!dir.mkpath(".")) {
                qDebug() << "Unable to create folder for cache; Path = \"" + dir.path() + "\"";
            }
        resourcePath = dir.path();

        connection = QSqlDatabase::addDatabase("QSQLITE");
        connection.setDatabaseName(dir.path() + QDir::separator() + "storage");
        if (!connection.open()) {
            qDebug() << "Unable to create database;\n Error: \"" << connection.lastError().text()
                     << "\"";
            return false;
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
            insertTimer = new QTimer(this);
            insertTimer->setInterval(1000);
            connect(insertTimer,&QTimer::timeout,this,&Cache::flushInsertQueue);
            return true;
        }
    }
private:
    void flushInsertQueue() {
        connection.transaction();
        for (auto & it :insertQueue)
            it.exec();
        connection.commit();
        insertQueue.clear();
    }
public:
    bool isOpened() const {
        return connection.isOpen();
    }
    template<class Resource>
    bool Save(CacheType type,const Resource& r) {
        using namespace internal;
        QSqlQuery insert;
        insert.prepare("REPLACE INTO " + connection.tables()[0] +
                QString("(id,type,data) VALUES (%1,%2,'%3') ").arg(r.id).arg(static_cast<int>(type))
                .arg(QString(JsonParser::i().toJson(r).data())));
        insertQueue.push_back(insert);
        return true;
    }
    template<class Resource>
    void Get(CacheType type) {
        using namespace  internal;

        QSqlQuery q("SELECT * FROM " + connection.tables()[0] +
                QString(" WHERE type = %1").arg(static_cast<int>(type)));

        if (q.lastError().type() != QSqlError::NoError) {
            qDebug() << "Error in \"Get\" function: " << q.lastError().text();
            return;
        }
        while (q.next()) {
           emit cacheData(QVariant::fromValue(
                              JsonParser::i().fromJson<Resource>(q.value("data").toString().toStdString())),type);
        }
    }
/*
    template<class Resource>
    QList<Resource> Get(CacheType type, const QString& condition) {
        QSqlQuery q("SELECT * FROM " + connection.tables()[0] +
                QString(" WHERE type = %1 AND %2").arg(static_cast<int>(type)).arg(condition));

        if (q.lastError().type() != QSqlError::NoError) {
            qDebug() << "Error in \"Get\" function: " << q.lastError().text();
            return {};
        }
        QList<Resource> items;
        using namespace internal;
        while (q.next()) {
            items.push_back(JsonParser::i().fromJson<Resource>(q.value("data").toString().toStdString().data()));
        }
        return items;
    }
    */
    bool Delete(CacheType type, const QString& condition = {}) {
        QSqlQuery q("DELETE FROM " + connection.tables()[0] +
                (condition.size() ? " WHERE " + condition
                                    + " AND type = " + QString::number(static_cast<int>(type))
                                  : ""));
        if (q.lastError().type() != QSqlError::NoError) {
            qDebug() << "Error in \"Delete\" function: " << q.lastError().text();
            return false;
        }
        return true;
    }
signals:
    void cacheHit(bool b);
    void cacheData(const QVariant& v,CacheType type);

public slots:
    void cacheCheck(CacheType type) {
        QSqlQuery q("SELECT count(*) as result FROM " + connection.tables()[0] +
                QString(" WHERE type = %1").arg(static_cast<uint32_t>(type)));
        if (q.lastError().type() != QSqlError::NoError) {
            emit cacheHit(false);
            return;
        } else {
            while (q.next())
                if (q.value("result").toInt() > 0) {
                    emit cacheHit(true);
                    return;
                }
        }
        emit cacheHit(false);
    }
    void cacheGet(CacheType type) {
        switch (type) {
            case CacheType::SEARCH_GROUP:
            case CacheType::SEARCH_TEACHER:
                return Get<internal::Timetable>(type);
            default:
                break;
        }
    }
    void SaveSlotGroup(const QVariant& v) {
        Save<internal::Timetable>(CacheType::SEARCH_GROUP,qvariant_cast<internal::Timetable>(v));
        insertTimer->start();
    }
    void SaveSlotTeacher(const QVariant& v) {
        Save<internal::Timetable>(CacheType::SEARCH_TEACHER,qvariant_cast<internal::Timetable>(v));
        insertTimer->start();
    }
};
}
