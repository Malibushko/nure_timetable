#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSettings>
#include "../internal/api_structs.h"
#include "../internal/material.h"
namespace timetable {
class SettingsModel : public QAbstractListModel {
    Q_OBJECT
    mutable QSettings settings;
public:
    SettingsModel(QObject* /* parent */= nullptr) {
        qDebug() << settings.fileName();
        if (settings.value("initialized",false) == false) {
            defaultSettings();
        }
    }
    Q_INVOKABLE void defaultSettings() {
        settings.beginGroup(tr("styling"));
        settings.setValue(tr("app_theme"),Material::color(Material::Green));
        settings.setValue(tr("app_accent"),Material::color(Material::Green));
        settings.setValue(tr("app_primary"),Material::color(Material::Green));
        settings.setValue(tr("night_mode"),false);
        settings.endGroup();

        settings.beginGroup(tr("timetable_styling"));
        settings.setValue(tr("zal_color"),QColor("#C2A0B8"));
        settings.setValue(tr("lb_color"),QColor("#CDCCFF"));
        settings.setValue(tr("lc_color"),QColor("#FEFEEA"));
        settings.setValue(tr("pz_color"),QColor("#DAED9D"));
        settings.endGroup();

        settings.beginGroup(tr("graphics"));
        settings.setValue(tr("animations"),true);
        settings.setValue(tr("caching"),true);
        settings.endGroup();

        settings.beginGroup(tr("miscellaneous"));
        settings.setValue(tr("language"),"eng");
        settings.setValue(tr("autoupdating"),false);
        settings.endGroup();

        settings.setValue("initialized",true);
        settings.sync();
    }
    Q_INVOKABLE QVariant value(const QString& group,
                               const QString& key,
                               const QVariant& defaultValue = QVariant{}) const {
        settings.beginGroup(group);
        auto value = settings.value(key,defaultValue);
        settings.endGroup();
        qDebug() << group << key << value;
        return value;
    }
    int columnCount(const QModelIndex & /* parent */) const override {
        return 1;
    }
    int rowCount(const QModelIndex& = /* parent */{}) const override {
        return settings.childGroups().size();
    }
    Q_INVOKABLE void updateSetting(const QString& group,
                                   const QString& name,
                                   const QVariant& value) {
        settings.beginGroup(group);
        settings.setValue(name,value);
        settings.endGroup();
        settings.sync();
        emit valueChanged(group,name,value);
    }
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override {
        if (index.row() < 0 || index.row() >= settings.childGroups().size()) {
            return {};
        }
        QString groups = settings.childGroups()[index.row()];
        switch (role) {
            case Qt::UserRole:
                return groups;
            case Qt::UserRole+1:
                settings.beginGroup(groups);
                QVariant keys = settings.childKeys();
                settings.endGroup();
                return keys;
        }
        return {};
    }
    QHash<int,QByteArray> roleNames() const override {
        QHash<int,QByteArray> roles {
            {Qt::UserRole,"group"},
            {Qt::UserRole+1,"settings"}
        };
        return roles;
    }
signals:
    void valueChanged(const QString& group,
                      const QString& key,
                      const QVariant& value);
};
}
