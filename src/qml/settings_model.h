#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include "../internal/api_structs.h"
#include "../internal/material.h"
namespace timetable {
class SettingsModel : public QAbstractListModel {
    Q_OBJECT
    mutable QSettings settings;
    QTranslator language;
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
        qApp->installTranslator(&language);
        setLanguage("");
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
        setIfNotExist("language","English;Русский",forceDefault);
        setIfNotExist("autoupdating",false,forceDefault);
        settings.endGroup();

        settings.beginGroup("button_callbacks");
        setIfNotExist("clear_cache_btn","btn");
        setIfNotExist("restore_default_btn","btn");
        settings.endGroup();

        setIfNotExist("chosen_language","ru");
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
    Q_INVOKABLE void setLanguage(const QString& lang) {
        if (lang == "English") {
            settings.setValue("chosen_language","eng");
        }
        else if (lang == "Русский") {
            settings.setValue("chosen_language","ru");
        }
        language.load(settings.value("chosen_language").toString());
        qDebug() << settings.value("chosen_language").toString();
    }
    Q_INVOKABLE QString getLanguage() const {
        QString lng = settings.value("chosen_language").toString();
        if (lng == "eng")
            return "English";
        else if (lng == "ru")
            return "Русский";
        else
            return "undefined";
    }
    Q_INVOKABLE QVariant value(const QString& key) const {
        return settings.value(key);
    }
    Q_INVOKABLE QString mapSettings(const QString& name) const {
        const QMap<QString,QString> settingsMapper{
            {"styling",tr("Styling")},
            {"app_theme",tr("Theme color")},
            {"app_accent",tr("Accent color")},
            {"app_primary",tr("Primary color")},
            {"night_mode",tr("Night mode")},
            {"timetable_styling",tr("Timetable styling")},
            {"zal_color",tr("Credit")},
            {"lb_color",tr("Laboratory work")},
            {"lc_color",tr("Lecture")},
            {"pz_color",tr("Practical work")},
            {"graphics",tr("Perfomance")},
            {"animations",tr("Animations")},
            {"caching",tr("Caching")},
            {"miscellaneous",tr("Miscellaneous")},
            {"language",tr("Language")},
            {"autoupdating",tr("Autoupdating")},
            {"button_callbacks",tr("Restoring")},
            {"clear_cache_btn",tr("Clear cache")},
            {"restore_default_btn",tr("Reset settings")}
        };
        return settingsMapper[name];
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
        default:
            break;
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
