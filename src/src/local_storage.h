#pragma once
#include <QObject>
#include <QtSql>
#include <QDateTime>
#include <QAbstractListModel>

#include "../internal/api_structs.h"
#include "database.h"

namespace timetable {
/**
 * @brief The LocalStorage class
 *
 * Wrapper class over Database for usage in QML code
 */
class LocalStorage : public QObject
{
   Q_OBJECT
public:
   /**
    * @brief Save data to persistent storage
    * @param type Type of table to save to
    * @param data Data to save (should be JSON serializable)
    * \note Asynchnorous operation
    */
   Q_INVOKABLE void save( TableType type, const QVariant& data );
   /**
    * @brief Optimized version of INSERT query, adapted for large number of data
    * @param type Type of table to save to
    * @param data List of items to save
    *
    * \note SQLLite is limited with 60 queries per second, so
    * we have to use transaction to insert large amount of data, otherwise
    * it would take hours of time
    *
    * \note In my code this function is called only twice - after first load,
    * to cache lists of timetables
    */
   Q_INVOKABLE void transactionSave( TableType type, const QVariantList& data );
   /**
    * @brief Get all data of table
    * @param type Type of table to select from
    * @return List of data
    * \note Blocking operation
    */
   Q_INVOKABLE QVariantList get( TableType type, QString condition = "" );
   /**
    * @brief Delete item from table
    * @param type Type of table to delete from
    * @param id Id of item to delete
    */
   Q_INVOKABLE void delete_( TableType type, int id );
   /**
    * @brief Creates new timetable from parts
    * @param id Timetable id
    * @param title Timetable title
    * @param isTeacher Boolean flag representing if timetable is teacher timetable or group
    * @param lessons List of lessons in timetable
    * @return QVariant containing SavedTimetable object
    */
   Q_INVOKABLE QVariant createTimetable(
      int id, QString title, bool isTeacher, const QVariantList& lessons, const QVariantList& hideLessons ) const;
   /**
    * @brief Begins transaction
    */
   Q_INVOKABLE void beginTransaction();
   /**
    * @brief Commits transaction
    */
   Q_INVOKABLE void endTransaction();
   /**
    * @brief Clear all storage records
    * \note Asynchronous operation
    */
   Q_INVOKABLE void clearStorage();
signals:
   /**
    * @brief Signal is emited when error happens
    * @param String representation of error
    * \note Used only for showing to the user
    */
   void error( const QString& message );
   /**
    * @brief Signal is emitted if operation was successfull
    * @param Message containing desctiption
    * \note Used only for showing to the user
    */
   void success( const QString& message );
};

} // namespace timetable
