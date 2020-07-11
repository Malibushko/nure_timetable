#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "../internal/api_structs.h"

namespace timetable {
/**
 * @brief The SavedTimetableModel class
 *
 * Represents model for displaing Timetable saved
 * by user
 */
class SavedTimetableModel : public QAbstractListModel {
    Q_OBJECT
    QList<internal::SavedTimetable> m_timetables; /**< List of Timetables saved by user*/
public:
    SavedTimetableModel(QObject* /* parent */= nullptr);
    /**
     * @brief Looks for timetable in a list
     * @param Id of timetable to look for
     * @return -1 if timetable not found, otherwise it`s position in the list
     */
    Q_INVOKABLE int find(int timetableId);
    /**
     * @brief Clear model and update QML view
     */
    Q_INVOKABLE void clear();
    /**
     * @brief Fills the model with items
     * @param List of QVariant that contains SavedTimetable objects
     */
    Q_INVOKABLE void setItems(const QVariantList& items);
    /**
     * @brief Replaces the item in timetable
     * @param New item
     * \note Comparison is made by comparing objects id`s
     */
    Q_INVOKABLE void replaceItem(const QVariant& item);
    /**
     * @brief Add new item to model
     * @param item to add
     * \note Item is also serialized and added to the database
     */
    Q_INVOKABLE void addItem(const QVariant& item);
    /**
     * @brief Removes item from model
     * @param id of item to remove
     * \note Item is also removed from the database
     */
    Q_INVOKABLE void removeItem(int id);
    /**
     * @brief Maps QDateTime to string representation
     * @param QDateTime object to map
     * @return QString object containing string representation of param
     * \note Works only for two days: Today and Yesterday, others have no sense to map
     * \note Definition moved to header because QLinguist does not add translation otherwise
     */
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

    // override methods
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};
}
