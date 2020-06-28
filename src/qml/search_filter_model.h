#pragma once
#include <QSortFilterProxyModel>
#include "search_model.h"

namespace timetable {
class SortFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    SearchItemModel * m_source;
public:
    SortFilterModel();
    Q_INVOKABLE void setFilterString(const QString& filter);
};
}
