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
private:
    void setIfNotExist(const QString& key,const QVariant& value,bool forceDefault = false) {
        if (forceDefault || settings.value(key) == QVariant{}) {
            settings.setValue(key,value);
        }
    }
public:
    SettingsModel(QObject* /* parent */= nullptr) {
        qDebug() << settings.fileName();
        loadSettings();
    }
    Q_INVOKABLE void loadSettings(bool forceDefault = false) {
        settings.beginGroup("styling");
        setIfNotExist("app_theme",Material::color(Material::Green),forceDefault);
        setIfNotExist("app_accent",Material::color(Material::Green),forceDefault);
        setIfNotExist("app_primary",Material::color(Material::Green),forceDefault);
        setIfNotExist("night_mode",false,forceDefault);
        settings.endGroup();

        settings.beginGroup("timetable_styling");
        setIfNotExist("zal_color",QColor("#C2A0B8"),forceDefault);
        setIfNotExist("lb_color",QColor("#CDCCFF"),forceDefault);
        setIfNotExist("lc_color",QColor("#FEFEEA"),forceDefault);
        setIfNotExist("pz_color",QColor("#DAED9D"),forceDefault);
        settings.endGroup();

        settings.beginGroup(tr("graphics"));
        setIfNotExist("animations",true,forceDefault);
        setIfNotExist("caching",true,forceDefault);
        settings.endGroup();

        settings.beginGroup("miscellaneous");
        setIfNotExist("language","eng",forceDefault);
        setIfNotExist("autoupdating",false,forceDefault);
        settings.endGroup();

        settings.beginGroup("button_callbacks");
        setIfNotExist("clear_cache_btn","btn");
        setIfNotExist("restore_default_btn","btn");
        settings.endGroup();

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
        case Qt::UserRole: {
            return groups;
        }
        case Qt::UserRole+1: {
            settings.beginGroup(groups);
            QVariant keys = settings.childKeys();
            settings.endGroup();
            return keys;
        }
        case Qt::UserRole+2: {
            return groups == "button_callbacks";
        }
        }
        return {};
    }
    QHash<int,QByteArray> roleNames() const override {
        QHash<int,QByteArray> roles {
            {Qt::UserRole,"group"},
            {Qt::UserRole+1,"settings"},
            {Qt::UserRole+2,"is_functional"}
        };
        return roles;
    }
signals:
    void valueChanged(const QString& group,
                      const QString& key,
                      const QVariant& value);
};
}
