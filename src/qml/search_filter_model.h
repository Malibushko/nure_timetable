#pragma once
#include <QSortFilterProxyModel>
#include "search_model.h"

namespace timetable {
/**
 * @brief The SortFilterModel class
 * Model used by SearchComponent (QML) to sort and filter items
 *
 * \note Comparison is always case insensitive
 */
class SortFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    SearchItemModel * m_source; /**< Source model */
    QString m_filter; /** Filter set by user */
public:
    SortFilterModel();
    Q_INVOKABLE void setFilterString(const QString& m_filter);
};
}
