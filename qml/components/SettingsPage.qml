import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import "../styles"
import lib 1.0
import Qt.labs.qmlmodels 1.0

Page {
    property alias modelRef: settingsModel

    ListView {
        anchors.fill: parent
        model: SettingsModel {
            id: settingsModel
            onValueChanged: {
                switch (key) {
                case "app_theme":
                    appSettings.themeColor = value;
                    break;
                case "app_accent":
                    appSettings.accentColor = value;
                    break;
                case "app_primary":
                    appSettings.primaryColor = value;
                    break;
                case "night_mode":
                    appSettings.appTheme = (value ? Material.Dark : Material.Light)
                    break;
                }
            }
        }
        delegate: Rectangle {
            width: parent.width
            height: appSettings.rowHeight
            color: appSettings.componentColor
            BottomBorder {
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: appSettings.animationsEnabled
                onHoveredChanged: {
                    parent.color = containsMouse ? Qt.darker(appSettings.componentColor,1.05) :
                                                   appSettings.componentColor;
                    parent.border.color = containsMouse ? appSettings.accentColor : "transparent"
                    parent.border.width = containsMouse
                }
                onClicked: {
                        settingsGroup.modelRef.setGroup(model.group)
                        settingsGroup.modelRef.setItems(model.settings)
                        mainView.push(settingsGroup);
                }
            }
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: appSettings.margin
                anchors.rightMargin: appSettings.margin
                StyledText {
                    text: settingsModel.mapSettings(model.group)
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
            }
        }
    }
}
