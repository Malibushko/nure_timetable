#pragma once
#include <QString>
#include <QStringList>

namespace timetable::internal {
/**
 * @brief The QueryBuilder class
 *
 * Helper class that constructs SQL queries
 */
class QueryBuilder {
    QString m_query; /**< SQL query */
public:
    /**
     * @brief QueryBuilder
     * Default ctor without params
     */
    explicit QueryBuilder();
    /**
     * @brief Get built SQL Query
     * @return Built SQL query
     */
    QString query() const;
    /**
     * @brief Add "INSERT INTO" to SQL query
     * @return reference to QueryBuilder object
     */
    QueryBuilder& insert();
    /**
     * @brief Add columns to insert for INSERT query
     * \note Only use with INSERT queries
     * @param Columns to add
     * @return reference to QueryBuilder object
     */
    QueryBuilder& columns(const QStringList& columns);
    /**
     * @brief Add values to insert for INSERT query
     * @param values to add
     * @return reference to QueryBuilder object
     */
    QueryBuilder& values(const QStringList& values);
    /**
     * @brief create UPDATE SQL query
     * @return reference to QueryBuilder object
     */
    QueryBuilder& update();
    /**
     * @brief create REPLACE SQL query
     * @return reference to QueryBuilder object
     * \note Not all SQL drivers supports REPLACE query
     */
    QueryBuilder& replace();
    /**
     * @brief Append "AND" to SQL query
     * @param value to append
     * @return reference to QueryBuilder object
     */
    QueryBuilder& and_(const QString& s);
    /**
     * @brief Create "DELETE" SQL query
     * @return reference to QueryBuilder object
     */
    QueryBuilder& delete_();

    /**
      @brief create "SELECT" SQL query
     * @return reference to QueryBuilder object
     */
    QueryBuilder& select(const QStringList& columns = {"*"});

    /**
     * @brief append "TABLE <name>" to SQL query
     * @param Name of table to append
     * @return reference to QueryBuilder object
     */
    QueryBuilder& table(const QString& table);

    /**
     * @brief create "SET" SQL query
     * @param Values to set
     * @return reference to QueryBuilder object
     */
    QueryBuilder& set(const QStringList& values);

    /**
     * @brief Append "WHERE <condition>" to SQL query
     * @param Condition to append
     * @return reference to QueryBuilder object
     */
    QueryBuilder& where(const QString& condition);;
};
}
