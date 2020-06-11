#pragma once
#include <QString>
#include <QStringList>

namespace timetable::internal {
class QueryBuilder {
    QString q;
public:
    explicit QueryBuilder() {}
    QString query() const {
        return q.trimmed();
    }
    QueryBuilder& insert() {
        q += "INSERT INTO ";
        return *this;
    }
    QueryBuilder& columns(const QStringList& columns) {
        q += " ("+ columns.join(',') + ')';
        return *this;
    }
    QueryBuilder& values(const QStringList& values) {
        q += " VALUES (" + values.join(',') + ')';
        return *this;
    }
    QueryBuilder& update() {
        q += "UPDATE ";
        return *this;
    }
    QueryBuilder& replace() {
        q += "REPLACE INTO ";
        return *this;
    }
    QueryBuilder& and_(const QString& s) {
        q += " AND " + s;
        return *this;
    }
    QueryBuilder& delete_() {
        q += "DELETE FROM ";
        return *this;
    }
    QueryBuilder& select(const QStringList& columns = {"*"}) {
        q += "SELECT " + columns.join(',')+" FROM ";
        return *this;
    }
    QueryBuilder& table(const QString& table) {
        q += ' ' + table + ' ';
        return *this;
    }
    QueryBuilder& set(const QStringList& values) {
        q += " SET " + values.join(',');
        return *this;
    }
    QueryBuilder& where(const QString& condition) {
        q += " WHERE " + condition + ' ';
        return *this;
    };
};
}
