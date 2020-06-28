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
    Q_INVOKABLE QString dayToString(QDateTime day) const {
        QDateTime today = QDateTime::currentDateTime();
        QString result;
        int dayDifference = day.daysTo(today);
        switch (dayDifference) {
        case 0:
            result = tr("Today");
            break;
        case 1:
            result = tr("Yesterday");
            break;
        default:
            break;
        }
        result += tr(" at ") +  day.time().toString("hh:mm");
        return result;
    }
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};
}
