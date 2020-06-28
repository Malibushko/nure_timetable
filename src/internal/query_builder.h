#pragma once
#include <QString>
#include <QStringList>

namespace timetable::internal {
class QueryBuilder {
    QString q;
public:
    explicit QueryBuilder();
    QString query() const;
    QueryBuilder& insert();
    QueryBuilder& columns(const QStringList& columns);
    QueryBuilder& values(const QStringList& values);
    QueryBuilder& update();
    QueryBuilder& replace();
    QueryBuilder& and_(const QString& s);
    QueryBuilder& delete_();
    QueryBuilder& select(const QStringList& columns = {"*"});
    QueryBuilder& table(const QString& table);
    QueryBuilder& set(const QStringList& values);
    QueryBuilder& where(const QString& condition);;
};
}
