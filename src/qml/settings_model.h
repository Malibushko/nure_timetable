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
    void setIfNotExist(const QString& key,const QVariant& value,bool forceDefault = false);
public:
    SettingsModel(QObject* /* parent */= nullptr);
    Q_INVOKABLE void loadSettings(bool forceDefault = false);
    Q_INVOKABLE QVariant value(const QString& group,
                               const QString& key,
                               const QVariant& defaultValue = QVariant{}) const;
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    Q_INVOKABLE void updateSetting(const QString& group,
                                   const QString& name,
                                   const QVariant& value);
    Q_INVOKABLE void setLanguage(const QString& lang);
    Q_INVOKABLE QString getLanguage() const;
    // moved to header because translation doesnt work while definition in .cpp
    Q_INVOKABLE QString mapSettings(const QString &name) const {
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
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
signals:
    void valueChanged(const QString& group,
                      const QString& key,
                      const QVariant& value);
};
}
