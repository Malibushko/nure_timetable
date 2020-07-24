#include "search_model.h"

namespace timetable {

SearchItemModel::SearchItemModel(QObject *) {
}

bool SearchItemModel::initialized() const {
    return m_timetables.size();
}

void SearchItemModel::setItems(const QVariantList &items) {
    beginResetModel();

    m_timetables.clear();
    m_timetables.reserve(items.size());

    for (const auto & item : items) {
        m_timetables.push_back(qvariant_cast<internal::Timetable>(item));
    }

    endResetModel();
}

void SearchItemModel::addItem(const QVariant &item) {
    beginInsertRows({},m_timetables.size(),m_timetables.size());

    m_timetables.push_back(qvariant_cast<internal::Timetable>(item));
    endInsertRows();
}


int SearchItemModel::rowCount(const QModelIndex &) const {
    return m_timetables.size();
}

QVariant SearchItemModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_timetables.size()) {
        return {};
    }
    const internal::Timetable& item = m_timetables[index.row()];
    switch (role) {
    case SearchRoles::ID:
        return item.id;
    case SearchRoles::TITLE:
        return item.title;
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> SearchItemModel::roleNames() const {
    QHash<int,QByteArray> roles {
        {SearchRoles::ID,"timetable_id"},
        {SearchRoles::TITLE,"title"}
    };

    return roles;
}

}
