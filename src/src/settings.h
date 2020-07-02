#pragma once
#include <QObject>
#include <QSettings>
#include <QDebug>
#include "internal/material.h"
namespace SETTINGS_TYPE {
Q_NAMESPACE
enum SETTING_TYPE {
    UNDEFINED = 0,
    APP_THEME = 10,
    APP_ACCENT,
    APP_PRIMARY,
    NIGHT_MODE,
    ZAL_COLOR,
    LB_COLOR,
    LC_COLOR,
    PZ_COLOR,
    ANIMATIONS,
    CACHING,
    LANGUAGE,
    AUTOUPDATING,
    CLEAR_CACHE_BTN,
    RESTORE_DEFAULT_BTN,
    CHOSEN_LANGUAGE,
    SHOW_TIMER
};
Q_ENUM_NS(SETTING_TYPE)
}
namespace SETTINGS_GROUP {
Q_NAMESPACE
enum SETTING_GROUP {
    UNDEFINED = 99,
    STYLING = 100,
    TIMETABLE_STYLING,
    MISCELLANEOUS,
    GRAPHICS,
    BTN_CALLBACKS
};

Q_ENUM_NS(SETTING_GROUP)
}
namespace CONTROL_TYPE {
Q_NAMESPACE
enum CONTROL {
    BTN = 150,
    LIST,
    SWITCH,
    COLOR,
    NUMBER
};
Q_ENUM_NS(CONTROL)
}

namespace timetable {
class Settings : public QObject {
    Q_OBJECT
    mutable QSettings settings;
private:
    void setIfNotExist(int key,const QVariant& value,bool forceDefault = false) {
        if (forceDefault || settings.value(QString::number(key)) == QVariant{}) {
            settings.setValue(QString::number(key),value);
        }
    }
public:
    Settings(QObject * parent = nullptr) : QObject{parent} {
        load();
        settings.sync();
    }
    Q_INVOKABLE QStringList groups() const {
        return settings.childGroups();
    }
    Q_INVOKABLE QVariantList groupKeys(int groupName) const {
        settings.beginGroup(QString::number(groupName));
        QVariantList keys;
        keys.reserve(settings.allKeys().size());
        for (const auto & setting : settings.allKeys()) {
            keys.push_back(QVariant::fromValue(QPair<int,QVariant>(setting.toInt(),settings.value(setting))));
        }
        settings.endGroup();
        return keys;
    }
    Q_INVOKABLE void     load(bool forceDefault = false) {
        settings.beginGroup(QString::number(SETTINGS_GROUP::STYLING));
        setIfNotExist(SETTINGS_TYPE::APP_THEME,Material::color(Material::Green),forceDefault);
        setIfNotExist(SETTINGS_TYPE::APP_ACCENT,Material::color(Material::Green),forceDefault);
        setIfNotExist(SETTINGS_TYPE::APP_PRIMARY,Material::color(Material::Green),forceDefault);
        setIfNotExist(SETTINGS_TYPE::NIGHT_MODE,false,forceDefault);
        settings.endGroup();

        settings.beginGroup(QString::number(SETTINGS_GROUP::TIMETABLE_STYLING));
        setIfNotExist(SETTINGS_TYPE::ZAL_COLOR,QColor("#C2A0B8"),forceDefault);
        setIfNotExist(SETTINGS_TYPE::LB_COLOR,QColor("#CDCCFF"),forceDefault);
        setIfNotExist(SETTINGS_TYPE::LC_COLOR,QColor("#FEFEEA"),forceDefault);
        setIfNotExist(SETTINGS_TYPE::PZ_COLOR,QColor("#DAED9D"),forceDefault);
        setIfNotExist(SETTINGS_TYPE::SHOW_TIMER,true,forceDefault);
        settings.endGroup();

        settings.beginGroup(QString::number(SETTINGS_GROUP::GRAPHICS));
        setIfNotExist(SETTINGS_TYPE::ANIMATIONS,true,forceDefault);
        setIfNotExist(SETTINGS_TYPE::CACHING,true,forceDefault);
        settings.endGroup();

        settings.beginGroup(QString::number(SETTINGS_GROUP::MISCELLANEOUS));
        setIfNotExist(SETTINGS_TYPE::LANGUAGE,"English;Русский",forceDefault);
        setIfNotExist(SETTINGS_TYPE::AUTOUPDATING,false,forceDefault);
        settings.endGroup();

        settings.beginGroup(QString::number(SETTINGS_GROUP::BTN_CALLBACKS));
        setIfNotExist(SETTINGS_TYPE::CLEAR_CACHE_BTN,"btn",forceDefault);
        setIfNotExist(SETTINGS_TYPE::RESTORE_DEFAULT_BTN,"btn",forceDefault);
        settings.endGroup();

        setIfNotExist(SETTINGS_TYPE::CHOSEN_LANGUAGE,"ru",forceDefault);
        settings.sync();
    }
    Q_INVOKABLE QVariant value(const QString& group,
                               const QString& key,
                               const QVariant& defaultValue = QVariant{}) const {
        if (group.size())
            settings.beginGroup(group);

        auto value = settings.value(key,defaultValue);

        if (group.size())
            settings.endGroup();
        return value;
    }

    Q_INVOKABLE void     update(int group,
                                int name,
                                const QVariant& value) {
        if (group != -1)
            settings.beginGroup(QString::number(group));

        settings.setValue(QString::number(name),value);

        if (group != -1)
            settings.endGroup();
        settings.sync();
        emit valueChanged(group,name,value);
    }
    Q_INVOKABLE QString  stringify(int type) const {
        switch (type) {
            case SETTINGS_TYPE::UNDEFINED:
                return tr("Undefined type");
            case SETTINGS_GROUP::UNDEFINED:
                return tr("Undefined group");
            case SETTINGS_GROUP::STYLING:
                return tr("Styling");
            case SETTINGS_TYPE::APP_THEME:
                return tr("Theme color");
            case SETTINGS_TYPE::APP_ACCENT:
                return tr("Accent color");
            case SETTINGS_TYPE::APP_PRIMARY:
                return tr("Primary color");
            case SETTINGS_TYPE::NIGHT_MODE:
                return tr("Night mode");
            case SETTINGS_GROUP::TIMETABLE_STYLING:
                return tr("Timetable styling");
            case SETTINGS_TYPE::ZAL_COLOR:
                return tr("Credit");
            case SETTINGS_TYPE::LB_COLOR:
                return tr("Laboratory work");
            case SETTINGS_TYPE::LC_COLOR:
                return tr("Lecture");
            case SETTINGS_TYPE::PZ_COLOR:
                return tr("Practical work");
            case SETTINGS_GROUP::GRAPHICS:
                return tr("Perfomance");
            case SETTINGS_TYPE::ANIMATIONS:
                return tr("Animations");
            case SETTINGS_TYPE::CACHING:
                return tr("Caching");
            case SETTINGS_GROUP::MISCELLANEOUS:
                return tr("Miscellaneous");
            case SETTINGS_TYPE::LANGUAGE:
                return tr("Language");
            case SETTINGS_TYPE::AUTOUPDATING:
                return tr("Autoupdating");
            case SETTINGS_GROUP::BTN_CALLBACKS:
                return tr("Restoring");
            case SETTINGS_TYPE::RESTORE_DEFAULT_BTN:
                return tr("Clear cache");
            case SETTINGS_TYPE::CLEAR_CACHE_BTN:
                return tr("Reset settings");
            case SETTINGS_TYPE::SHOW_TIMER:
                    return tr("Show timer");
            default:
                return "";
        };
    }
    Q_INVOKABLE QString stringify(const QString& type) const {
        return stringify(type.toInt());
    }
signals:
    void                 valueChanged(int group,
                                      int key,
                                      const QVariant& value);
};
}
