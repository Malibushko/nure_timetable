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
Q_NAMESPACE
/**
 * @brief Enum contains types of records for serialization
 */
enum TableType
{
    SEARCH_TEACHER = 1 << 1,  /// teacher timetable
    SEARCH_GROUP = 1 << 2,    /// group timetable
    SAVED_TIMETABLE = 1 << 4, /// saved timetable (with lessons)
    NOTE = 1 << 5             /// lesson note
};
Q_ENUM_NS( TableType )

using DefaultCallback = std::function< void( QVariant ) >;
/**
 * @brief The Database class
 *
 * Performs asynchronous operations with database
 * \note Singletone class
 */
class Database : public QObject
{
    Q_OBJECT
 private:
    QString         m_tableName; /// name of table (database is key-value)
    ExecutorService m_executor;  /// Asynchronous requests manager
 private:
    /**
     * @brief Database default constructor
     *
     * Create connection and initializes database
     * \note Blocking operation
     */
    explicit Database( QObject* obj = nullptr )
       : QObject{ obj }
    {
        QEventLoop waiter;
        m_executor.submit( []() { return QVariant::fromValue( Database::connect() ); } )
           .then( [&]( QVariant table ) {
               m_tableName = qvariant_cast< QString >( table );
               waiter.quit();
           } );
        waiter.exec();
    }
    /**
     * @brief Trying to create connection with database
     * @return Database table name if success, empty string otherwise
     * \note If database do not exist - creates one
     * \note "error" signal is emited if error happened
     */
    static QString connect()
    {
        // create dir with information
        QDir dir{ QStandardPaths::writableLocation( QStandardPaths::DataLocation ) };
        dir.cd( "storage" );
        if( !dir.exists() )
            dir.mkpath( "." );
        auto connection = QSqlDatabase::addDatabase( "QSQLITE" );
        connection.setDatabaseName( dir.path() + QDir::separator() + "storage" );
        if( !connection.open() )
        {
            qDebug() << "Unable to create database;\n Error: \""
                     << connection.lastError().text() << "\"";
            qDebug() << dir.path();
            return "";
        }
        else
        {
            qDebug() << dir.path();
            if( !connection.tables().size() )
                QSqlQuery q( "CREATE TABLE`kvalue` ("
                             "`id`	INTEGER NOT NULL,"
                             "`type`	INTEGER NOT NULL,"
                             "`data`	TEXT,"
                             "PRIMARY KEY(`id`, `type`)"
                             ");" );
            qDebug() << "Database opened; \nTables:" << connection.tables();
            return connection.tables().first();
        }
    }

 public:
    /**
     * @brief Database instance
     * @return Reference to database object
     */
    static Database& instance()
    {
        static Database db;
        return db;
    }

    /**
     * @brief Asynchronous insert operation
     * @param type Type of table
     * @param r Item to insert
     * @param cb Callback that will be called after insert
     */
    template< class Resource, class Callback = DefaultCallback >
    void Save( TableType type, const Resource& r, Callback&& cb = {} )
    {
        using namespace internal;
        QueryBuilder builder;
        builder.replace()
           .table( m_tableName )
           .values(
              { QString::number( r.id ), QString::number( static_cast< int >( type ) ),
                 '\'' + QString( JsonParser::i().toJson( r ).data() ) + '\'' } );
        m_executor
           .submit( [query = builder.query()]() {
               QSqlQuery q( query );
               qDebug() << query;
               return QVariant::fromValue( q.lastInsertId() );
           } )
           .then( cb );
    }
    /**
     * @brief Asynchonous SELECT query
     * @param type Table type
     * @param cb Callback that will be used to process data
     */
    template< class Resource, class Callback = DefaultCallback >
    void Get( TableType type, Callback&& cb = {} )
    {
        using namespace internal;
        QueryBuilder builder;
        builder.select()
           .table( m_tableName )
           .where( QString( "type = %1" ).arg( static_cast< int >( type ) ) );
        m_executor
           .submit( [query = builder.query()]() {
               QSqlQuery    q( query );
               QVariantList result;
               while( q.next() )
               {
                   result.push_back(
                      QVariant::fromValue( JsonParser::i().fromJson< Resource >(
                         q.value( "data" ).toString().toStdString().data() ) ) );
               }
               return QVariant::fromValue( result );
           } )
           .then( cb );
    }
    /**
     * Asynchronous SELECT query
     * @param type Table type
     * @param condition Condition for SQL SELECT query
     * @param cb Callback that will be used to process data
     */
    template< class Resource, class Callback = DefaultCallback >
    void Get( TableType type, const QString& condition, Callback&& cb = {} )
    {
        using namespace internal;
        QueryBuilder builder;
        builder.select()
           .table( m_tableName )
           .where( QString( "type = %1" ).arg( static_cast< int >( type ) ) );
        if( condition != "" )
            builder.and_( condition );

        m_executor
           .submit( [query = builder.query()]() {
               QSqlQuery    q( query );
               QVariantList result;
               while( q.next() )
               {
                   result.push_back(
                      QVariant::fromValue( JsonParser::i().fromJson< Resource >(
                         q.value( "data" ).toString().toStdString().data() ) ) );
               }
               return QVariant::fromValue( result );
           } )
           .then( cb );
    }
    /**
     * @brief Delete SQL Query
     * @param type Table type
     * @param cb Callback that will be called after data is deleted
     */
    template< class Callback = DefaultCallback >
    void Delete( TableType type, int id, Callback&& cb = {} )
    {
        using namespace internal;
        QueryBuilder builder;
        builder.delete_().table( m_tableName );
        if( id != -1 )
            builder.where( QString( "id = %1" ).arg( id ) )
               .and_( QString( "type = %1" ).arg( static_cast< int >( type ) ) );
        m_executor
           .submit( [query = builder.query()] {
               QSqlQuery q( query );
               qDebug() << query;
               return QVariant::fromValue( q.lastError().type() != QSqlError::NoError );
           } )
           .then( cb );
    }
    /**
     * @brief begins SQL transaction
     */
    void beginTransaction()
    {
        m_executor.submit( []() {
            return QVariant::fromValue( QSqlDatabase::database().transaction() );
        } );
    }
    /**
     * @brief commits SQL transaction
     */
    void commit()
    {
        m_executor
           .submit(
              [] { return QVariant::fromValue( QSqlDatabase::database().commit() ); } )
           .then( DefaultCallback{} );
    }
    /**
     * @brief rollbacks SQL transaction
     */
    void rollback()
    {
        m_executor
           .submit(
              [] { return QVariant::fromValue( QSqlDatabase::database().rollback() ); } )
           .then( DefaultCallback{} );
    }
    /**
     * @brief Drop all tables in database
     * \note database file is not deleted
     */
    void clearDatabase()
    {
        m_executor
           .submit( [&] {
               QSqlQuery q( "DROP TABLE kvalue" );
               if( q.lastError().type() != QSqlError::NoError )
               {
                   qDebug() << q.lastError().text();
               }
               else
               {
                   qDebug() << q.lastQuery();
                   this->connect();
               }
               return QVariant{};
           } )
           .then( DefaultCallback{} );
    }
 signals:
    /// @brief Signal is emited when database unable to create connection
    void unableToConnect();
    /**
     * @brief Signal is emited when some error happened
     * @param description String representation of error
     */
    void error( QString description );
};
} // namespace timetable
