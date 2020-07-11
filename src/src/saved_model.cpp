#include "qml/saved_model.h"

namespace timetable {
SavedTimetableModel::SavedTimetableModel(QObject *) {
}


void SavedTimetableModel::clear() {
    beginResetModel();
    m_timetables.clear();
    endResetModel();
}


int SavedTimetableModel::find(int timetableId) {
    auto iter = std::find_if(m_timetables.begin(),m_timetables.end(),[=](const auto& it){
        return it.id == timetableId;
    });
    return iter != m_timetables.end() ? std::distance(m_timetables.begin(),iter) : -1;
}


void SavedTimetableModel::setItems(const QVariantList &items) {
    beginResetModel();
    m_timetables.reserve(items.size());
    for (const QVariant& v : items) {
        m_timetables.push_back(qvariant_cast<internal::SavedTimetable>(v));
    }
    endResetModel();
}


void SavedTimetableModel::replaceItem(const QVariant &item) {
    decltype (auto) timetable = qvariant_cast<internal::SavedTimetable>(item);
    auto iter = std::find_if(m_timetables.begin(),m_timetables.end(),[&](const internal::SavedTimetable& t){
        return t.id == timetable.id;
    });
    if (iter != m_timetables.end()) {
        *iter = std::move(timetable);
        emit dataChanged(createIndex(std::distance(m_timetables.begin(),iter),1),
                         createIndex(std::distance(m_timetables.begin(),iter),1));
    }
}


void SavedTimetableModel::addItem(const QVariant &item) {
    beginInsertRows({},m_timetables.size(),m_timetables.size());
    m_timetables.push_back(qvariant_cast<internal::SavedTimetable>(item));
    endInsertRows();
}


void SavedTimetableModel::removeItem(int id) {
    auto it = std::find_if(m_timetables.begin(),m_timetables.end(),[=](const internal::SavedTimetable& s){
        return s.id == id;
    });
    if (it != m_timetables.end()) {
        beginRemoveRows({},std::distance(m_timetables.begin(),it),std::distance(m_timetables.begin(),it));
        m_timetables.erase(it);
        endRemoveRows();
    }
}


int SavedTimetableModel::columnCount(const QModelIndex &) const {
    return 1;
}


int SavedTimetableModel::rowCount(const QModelIndex &) const {
    return m_timetables.size();
}


QVariant SavedTimetableModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_timetables.size()) {
        return {};
    }
    decltype (auto) item = m_timetables[index.row()];
    switch (role) {
    case Qt::UserRole:
        return item.id;
    case Qt::UserRole+1:
        return item.title;
    case Qt::UserRole+2: {
        auto today = QDateTime::currentDateTime();
        if (auto updateDate = QDateTime::fromString(item.lastUpdate);
                updateDate.daysTo(today) < 2) {
            return dayToString(updateDate);
        } else
            return item.lastUpdate;
    }
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
    case Qt::UserRole+5: {
        return QDateTime::fromString(item.lastUpdate).secsTo(QDateTime::currentDateTime());
    }
    default:
        break;
    }
    return {};
}


QHash<int, QByteArray> SavedTimetableModel::roleNames() const {
    QHash<int,QByteArray> roles {
        {Qt::UserRole,"id"},
        {Qt::UserRole+1,"title"},
        {Qt::UserRole+2,"lastUpdate"},
        {Qt::UserRole+3,"lessons"},
        {Qt::UserRole+4,"isTeacher"},
        {Qt::UserRole+5,"timeSinceLastUpdate"}
    };
    return roles;
}

}
