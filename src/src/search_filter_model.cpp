#include "qml/search_filter_model.h"
namespace timetable {
SortFilterModel::SortFilterModel()  {
    m_source = new SearchItemModel(this);
    this->setSourceModel(m_source);
    this->setFilterRole(Qt::UserRole+1);
}


void SortFilterModel::setFilterString(const QString &filter) {
    this->setFilterRegExp(filter);
}
}