#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "../internal/api_structs.h"
namespace timetable {

class SearchItemModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<internal::Timetable> mTimetables;
    QRegularExpression filter;
public:
    SearchItemModel(QObject* /* parent */= nullptr);
    Q_INVOKABLE bool initialized() const;
    Q_INVOKABLE void setItems(const QVariantList& items);
    Q_INVOKABLE void addItem(const QVariant& item);
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};

}
