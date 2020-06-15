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
    SavedTimetableModel(QObject* /* parent */= nullptr) {
    }
    Q_INVOKABLE void setItems(const QVariantList& items) {
        beginResetModel();
        timetables.reserve(items.size());
        for (const QVariant& v : items) {
            timetables.push_back(qvariant_cast<internal::SavedTimetable>(v));
        }
        endResetModel();
    }
    Q_INVOKABLE void replaceItem(const QVariant& item) {
        decltype (auto) timetable = qvariant_cast<internal::SavedTimetable>(item);
        auto iter = std::find_if(timetables.begin(),timetables.end(),[&](const internal::SavedTimetable& t){
            return t.id == timetable.id;
        });
        if (iter != timetables.end()) {
            *iter = std::move(timetable);
            emit dataChanged(createIndex(std::distance(timetables.begin(),iter),1),
                             createIndex(std::distance(timetables.begin(),iter),1));
        }
    }
    Q_INVOKABLE void addItem(const QVariant& item) {
        beginInsertRows({},timetables.size(),timetables.size());
        timetables.push_back(qvariant_cast<internal::SavedTimetable>(item));
        endInsertRows();
    }
    Q_INVOKABLE void removeItem(int id) {
        auto it = std::find_if(timetables.begin(),timetables.end(),[=](const internal::SavedTimetable& s){
            return s.id == id;
        });
        if (it != timetables.end()) {
            beginRemoveRows({},std::distance(timetables.begin(),it),std::distance(timetables.begin(),it));
            timetables.erase(it);
            endRemoveRows();
        }
    }
    int columnCount(const QModelIndex & /* parent */) const override {
        return 1;
    }
    int rowCount(const QModelIndex& = /* parent */{}) const override {
        return timetables.size();
    }
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override {
        if (index.row() < 0 || index.row() >= timetables.size()) {
            return {};
        }
        decltype (auto) item = timetables[index.row()];
        switch (role) {
        case Qt::UserRole:
            return item.id;
        case Qt::UserRole+1:
            return item.title;
        case Qt::UserRole+2:
        {
            auto today = QDateTime::currentDateTime();
            if (auto updateDate = QDateTime::fromString(item.lastUpdate);
                    updateDate.daysTo(today) < 2) {
                QString day;
                int dayDifference = updateDate.daysTo(today);
                switch (dayDifference) {
                    case 0:
                     day = tr("Today");
                     break;
                    case 1:
                     day = tr("Yesterday");
                    break;
                }
                day += tr(" at ") +  updateDate.time().toString("hh:mm");
                return day;
            }
        }
        return item.lastUpdate;
        case Qt::UserRole+3: {
            QVariantList list;
            list.reserve(item.lessons.size());
            for (const auto & lesson : item.lessons)
                list.push_back(QVariant::fromValue(lesson));
            return QVariant::fromValue(list);
        }
        case Qt::UserRole+4: {
            return item.isTeacher;
        }
        }
        return {};
    }
    QHash<int,QByteArray> roleNames() const override {
        QHash<int,QByteArray> roles {
            {Qt::UserRole,"id"},
            {Qt::UserRole+1,"title"},
            {Qt::UserRole+2,"lastUpdate"},
            {Qt::UserRole+3,"lessons"},
            {Qt::UserRole+4,"isTeacher"}
        };
        return roles;
    }
};
}
