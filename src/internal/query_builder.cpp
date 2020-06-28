#include "query_builder.h"

namespace timetable::internal {

QueryBuilder::QueryBuilder() {}


QString QueryBuilder::query() const {
    return q.trimmed();
}


QueryBuilder &QueryBuilder::insert() {
    q += "INSERT INTO ";
    return *this;
}


QueryBuilder &QueryBuilder::columns(const QStringList &columns) {
    q += " ("+ columns.join(',') + ')';
    return *this;
}


QueryBuilder &QueryBuilder::values(const QStringList &values) {
    q += " VALUES (" + values.join(',') + ')';
    return *this;
}


QueryBuilder &QueryBuilder::update() {
    q += "UPDATE ";
    return *this;
}


QueryBuilder &QueryBuilder::replace() {
    q += "REPLACE INTO ";
    return *this;
}


QueryBuilder &QueryBuilder::and_(const QString &s) {
    q += " AND " + s;
    return *this;
}


QueryBuilder &QueryBuilder::delete_() {
    q += "DELETE FROM ";
    return *this;
}


QueryBuilder &QueryBuilder::select(const QStringList &columns) {
    q += "SELECT " + columns.join(',')+" FROM ";
    return *this;
}


QueryBuilder &QueryBuilder::table(const QString &table) {
    q += ' ' + table + ' ';
    return *this;
}


QueryBuilder &QueryBuilder::set(const QStringList &values) {
    q += " SET " + values.join(',');
    return *this;
}


QueryBuilder &QueryBuilder::where(const QString &condition) {
    q += " WHERE " + condition + ' ';
    return *this;
}

}
