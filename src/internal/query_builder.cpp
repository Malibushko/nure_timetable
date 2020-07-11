#include "query_builder.h"

namespace timetable::internal {

QueryBuilder::QueryBuilder() {}


QString QueryBuilder::query() const {
    return m_query.trimmed();
}


QueryBuilder &QueryBuilder::insert() {
    m_query += "INSERT INTO ";
    return *this;
}


QueryBuilder &QueryBuilder::columns(const QStringList &columns) {
    m_query += " ("+ columns.join(',') + ')';
    return *this;
}


QueryBuilder &QueryBuilder::values(const QStringList &values) {
    m_query += " VALUES (" + values.join(',') + ')';
    return *this;
}


QueryBuilder &QueryBuilder::update() {
    m_query += "UPDATE ";
    return *this;
}


QueryBuilder &QueryBuilder::replace() {
    m_query += "REPLACE INTO ";
    return *this;
}


QueryBuilder &QueryBuilder::and_(const QString &s) {
    m_query += " AND " + s;
    return *this;
}


QueryBuilder &QueryBuilder::delete_() {
    m_query += "DELETE FROM ";
    return *this;
}


QueryBuilder &QueryBuilder::select(const QStringList &columns) {
    m_query += "SELECT " + columns.join(',')+" FROM ";
    return *this;
}


QueryBuilder &QueryBuilder::table(const QString &table) {
    m_query += ' ' + table + ' ';
    return *this;
}


QueryBuilder &QueryBuilder::set(const QStringList &values) {
    m_query += " SET " + values.join(',');
    return *this;
}


QueryBuilder &QueryBuilder::where(const QString &condition) {
    m_query += " WHERE " + condition + ' ';
    return *this;
}

}
