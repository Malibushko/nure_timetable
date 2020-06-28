#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "../internal/api_structs.h"

namespace timetable {

class SavedTimetableModel : public QAbstractListModel {
    Q_OBJECT
    QList<internal::SavedTimetable> timetables;
public:
    SavedTimetableModel(QObject* /* parent */= nullptr);
    Q_INVOKABLE void clear();
    Q_INVOKABLE int find(int timetableId);
    Q_INVOKABLE void setItems(const QVariantList& items);
    Q_INVOKABLE void replaceItem(const QVariant& item);
    Q_INVOKABLE void addItem(const QVariant& item);
    Q_INVOKABLE void removeItem(int id);
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};
}
