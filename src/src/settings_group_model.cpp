#include "qml/settings_group_model.h"
#include "src/settings.h"
namespace timetable {

SettingsGroupModel::SettingsGroupModel(QObject *) {

}

void SettingsGroupModel::setItems(const QVariantList &otherSettings) {
    beginResetModel();
    settingTitles.clear();
    settingTitles.reserve(otherSettings.size());
    for (const QVariant& item : otherSettings) {
        settingTitles.push_back(qvariant_cast<QPair<int,QVariant>>(item));
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
            return CONTROL_TYPE::CONTROL::BTN;
        }
        if (name.second.canConvert<QString>()
                && name.second.toString().contains(';')) {
            return CONTROL_TYPE::CONTROL::LIST;
        }
        if (name.second.canConvert<bool>()) {
            return CONTROL_TYPE::CONTROL::SWITCH;
        }
        if (name.second.canConvert<int>()) {
            return CONTROL_TYPE::CONTROL::NUMBER;
        }
        if (name.second.canConvert<QColor>()) {
            return CONTROL_TYPE::CONTROL::COLOR;
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
