#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QDebug>
#include <QSettings>
#include "../internal/api_structs.h"
#include "../internal/material.h"

namespace timetable {
/**
 * @brief The SettingsGroupModel class
 *
 * Model for group of settings
 * Displayed as List of settings with different contols
 */
class SettingsGroupModel : public QAbstractListModel {
    Q_OBJECT
    QVector<QPair<int,QVariant>> m_settingTitles;
public:
    SettingsGroupModel(QObject* /* parent */= nullptr);
    Q_INVOKABLE void setItems(const QVariantList& otherSettings);

    // override methods
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};
}
