#include "qml/settings_group_model.h"
namespace timetable {

SettingsGroupModel::SettingsGroupModel(QObject *) {

}
void SettingsGroupModel::setGroup(const QString &s){
    settings.endGroup();
    settings.beginGroup(s);
}


QString SettingsGroupModel::getGroup() const {
    return settings.group();
}


void SettingsGroupModel::setItems(const QVariantList &otherSettings) {
    beginResetModel();
    settingTitles.clear();
    settingTitles.reserve(otherSettings.size());

    for (const QVariant& item : otherSettings) {
        QString key = qvariant_cast<QString>(item);
        QVariant  value = this->settings.value(key);
        settingTitles.push_back({key,value});
    }
    endResetModel();
}


int SettingsGroupModel::columnCount(const QModelIndex &) const {
    return 1;
}


int SettingsGroupModel::rowCount(const QModelIndex &) const {
    return settingTitles.size();
}


QVariant SettingsGroupModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= settingTitles.size()) {
        return {};
    }
    const auto& name = settingTitles[index.row()];
    switch (role) {
    case Qt::UserRole:
        return name.first;
    case Qt::UserRole+1:
        if (name.second.canConvert<QString>() && name.second.toString().contains(';'))
            return name.second.toString();
        if (name.second.canConvert<bool>())
            return name.second.toString() == "true";
        return name.second;
    case Qt::UserRole+2: {
        if (name.second.toString() == "btn") {
            return "btn";
        }
        if (name.second.canConvert<QString>()
                && name.second.toString().contains(';')) {
            return "list";
        }
        if (name.second.canConvert<bool>()) {
            return "slider";
        }
        if (name.second.canConvert<int>()) {
            return "number";
        }
        if (name.second.canConvert<QColor>()) {
            return "color";
        }
        return "none";
    }
    default:
        break;
    }
    return {};
}
QHash<int, QByteArray> SettingsGroupModel::roleNames() const {
    QHash<int,QByteArray> roles {
        {Qt::UserRole,"name"},
        {Qt::UserRole+1,"value"},
        {Qt::UserRole+2,"type"}
    };
    return roles;
}
}
