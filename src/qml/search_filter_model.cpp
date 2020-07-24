#include "qml/search_filter_model.h"
namespace timetable {
SortFilterModel::SortFilterModel()  {
    m_source = new SearchItemModel(this);
    setSourceModel(m_source);
    setFilterRole(Qt::UserRole+1);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filter.reserve(40); // just some random number in order to decrease reallocation count
}


void SortFilterModel::setFilterString(const QString &filter) {
    this->setFilterRegExp(filter);
}
}
