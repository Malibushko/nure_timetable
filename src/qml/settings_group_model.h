#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSettings>
#include "../internal/api_structs.h"
#include "../internal/material.h"

namespace timetable {
class SettingsGroupModel : public QAbstractListModel {
    Q_OBJECT
    mutable QSettings settings;
    QVector<QPair<QString,QVariant>> settingTitles;
public:
    SettingsGroupModel(QObject* /* parent */= nullptr) {

    }
    Q_INVOKABLE void setGroup(const QString& s){
        settings.endGroup();
        settings.beginGroup(s);
    }
    Q_INVOKABLE QString getGroup() const {
        return settings.group();
    }
    Q_INVOKABLE void setItems(const QVariantList& settings) {
        beginResetModel();
        settingTitles.clear();
        settingTitles.reserve(settings.size());

        for (const QVariant& item : settings) {
            QString key = qvariant_cast<QString>(item);
            QVariant  value = this->settings.value(key);
            settingTitles.push_back({key,value});
        }
        endResetModel();
    }
    int columnCount(const QModelIndex & /* parent */) const override {
        return 1;
    }
    int rowCount(const QModelIndex& = /* parent */{}) const override {
        return settingTitles.size();
    }
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override {
        if (index.row() < 0 || index.row() >= settingTitles.size()) {
            return {};
        }
        const auto& name = settingTitles[index.row()];
        switch (role) {
            case Qt::UserRole:
                return name.first;
            case Qt::UserRole+1:
                if (name.second.canConvert<bool>())
                    return name.second.toString() == "true";
                return name.second;
            case Qt::UserRole+2: {
                if (name.second.canConvert<bool>()) {
                    return "slider";
                }
                if (name.second.canConvert<int>()) {
                    return "number";
                }
                if (name.second.canConvert<QColor>()) {
                    return "color";
                }
                if (name.second.canConvert<QStringList>()) {
                    return "list";
                }
                return "none";
            }
        }
        return {};
    }
    QHash<int,QByteArray> roleNames() const override {
        QHash<int,QByteArray> roles {
            {Qt::UserRole,"name"},
            {Qt::UserRole+1,"value"},
            {Qt::UserRole+2,"type"}
        };
        return roles;
    }
};
}
