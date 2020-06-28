#include "qml/settings_model.h"

namespace timetable {

void SettingsModel::setIfNotExist(const QString &key, const QVariant &value, bool forceDefault) {
    if (forceDefault || settings.value(key) == QVariant{}) {
        settings.setValue(key,value);
    }
}

SettingsModel::SettingsModel(QObject *) {
    loadSettings();
    setLanguage("");
    qApp->installTranslator(&language);
}


void SettingsModel::loadSettings(bool forceDefault) {
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

    setIfNotExist("chosen_language","ru",forceDefault);
    settings.sync();
}


QVariant SettingsModel::value(const QString &group, const QString &key, const QVariant &defaultValue) const {
    settings.beginGroup(group);
    auto value = settings.value(key,defaultValue);
    settings.endGroup();
    qDebug() << group << key << value;
    return value;
}


int SettingsModel::columnCount(const QModelIndex &) const {
    return 1;
}


int SettingsModel::rowCount(const QModelIndex &) const {
    return settings.childGroups().size();
}


void SettingsModel::updateSetting(const QString &group, const QString &name, const QVariant &value) {
    settings.beginGroup(group);
    settings.setValue(name,value);
    settings.endGroup();
    settings.sync();
    emit valueChanged(group,name,value);
}


void SettingsModel::setLanguage(const QString &lang) {
    if (lang == "English") {
        settings.setValue("chosen_language","eng");
    }
    else if (lang == "Русский") {
        settings.setValue("chosen_language","ru");
    }
    language.load(settings.value("chosen_language").toString());
}

QString SettingsModel::getLanguage() const {
    QString lng = settings.value("chosen_language").toString();
    if (lng == "eng")
        return "English";
    else if (lng == "ru")
        return "Русский";
    else
        return "undefined";
}



QString SettingsModel::mapSettings(const QString &name) const {
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


QVariant SettingsModel::data(const QModelIndex &index, int role) const {
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

QHash<int, QByteArray> SettingsModel::roleNames() const {
    QHash<int,QByteArray> roles {
        {Qt::UserRole,"group"},
        {Qt::UserRole+1,"settings"},
        {Qt::UserRole+2,"is_functional"}
    };
    return roles;
}

}
