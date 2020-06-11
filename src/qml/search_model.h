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
    SearchItemModel(QObject* /* parent */= nullptr) {

    }
    Q_INVOKABLE bool initialized() const {
        return mTimetables.size();
    }
    Q_INVOKABLE void setFilter(const QString & s) {
        filter.setPattern(s);
        emit dataChanged(createIndex(0,0),createIndex(mTimetables.size(),0),{Qt::UserRole+2});
    }

    Q_INVOKABLE void setItems(const QVariantList& items) {
        beginResetModel();

        mTimetables.clear();
        mTimetables.reserve(items.size());

        for (const auto & item : items) {
            mTimetables.push_back(qvariant_cast<internal::Timetable>(item));
        }

        endResetModel();
    }
    Q_INVOKABLE void addItem(const QVariant& item) {
        beginInsertRows({},mTimetables.size(),mTimetables.size());

        mTimetables.push_back(qvariant_cast<internal::Timetable>(item));
        endInsertRows();
    }
    int columnCount(const QModelIndex & /* parent */) const override {
        return 1;
    }
    int rowCount(const QModelIndex& = /* parent */{}) const override {
        return mTimetables.size();
    }
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override {
        if (index.row() < 0 || index.row() >= mTimetables.size()) {
            return {};
        }
        const internal::Timetable& item = mTimetables[index.row()];
        switch (role) {
            case Qt::UserRole:
                return item.id;
            case Qt::UserRole+1:
                return item.title;
            case Qt::UserRole+2:
                return filter.match(item.title).hasMatch();
        }
        return {};
    }
    QHash<int,QByteArray> roleNames() const override {
        QHash<int,QByteArray> roles {
            {Qt::UserRole,"timetable_id"},
            {Qt::UserRole+1,"title"},
            {Qt::UserRole+2,"visible"}
        };
        return roles;
    }
};

}
