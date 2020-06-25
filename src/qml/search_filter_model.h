#pragma once
#include <QSortFilterProxyModel>
#include "search_model.h"
namespace timetable {
class SearchItemModel;

class SortFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    SearchItemModel * m_source;
public:
    SortFilterModel()  {
        m_source = new SearchItemModel(this);
        this->setSourceModel(m_source);
        this->setFilterRole(Qt::UserRole+1);
    }
    Q_INVOKABLE void setFilterString(const QString& filter) {
        this->setFilterRegExp(filter);
    }
};
}
