#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include "../internal/api_structs.h"

namespace timetable {
/**
 * @brief The SearchItemModel class
 *
 * Model that is used for searching timetable
 */
class SearchItemModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<internal::Timetable> m_timetables; /// list of timetables
public:
    SearchItemModel(QObject* /* parent */= nullptr);
    /**
     * @brief Check if model was initialized
     * @return True if model was initialized, false otherwise
     */
    Q_INVOKABLE bool initialized() const;
    /**
     * @brief Fill model with items
     * @param Timetable objects
     */
    Q_INVOKABLE void setItems(const QVariantList& items);
    /**
     * @brief Add Timetable to list
     * @param Timetable object in QVariant wrapper
     */
    Q_INVOKABLE void addItem(const QVariant& item);

    // override methods
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};

}
