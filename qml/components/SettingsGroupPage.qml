import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.0
import Qt.labs.settings 1.0
import "../styles"
import lib 1.0

Page {
    property alias modelRef: settingsModel
    Component {
        id: switchDelegate
        Switch {
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Component.onCompleted: {
                checked =  modelData
            }
            onCheckedChanged: {
                modelData = checked
            }
        }
    }
    Component {
        id: colorDelegate
        Rectangle {
            width: appSettings.rowHeight * 0.75
            height: width
            color: modelData
            MouseArea {
                anchors.fill: parent
                ColorDialog {
                    id: colorPicker
                    title: qsTr("Please, select a color");
                    onAccepted: {
                        modelData = colorPicker.color
                    }
                }
                onClicked: {
                    colorPicker.open()
                }
            }
        }
    }
    Component {
        id: numberDelegate
        SpinBox {
            value: 0
            onValueChanged: {
                modelData = value;
            }
        }
    }
    Component {
        id: listDelegate
        ComboBox {
            model: modelData
            onActivated:  {
                modelData = currentText
            }
        }
    }
    Component {
        id: btnDelegate
        RowButton {
            id: pressButton
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: appSettings.margin
            icon.source: "qrc:///qml/icons/security.svg"
            onClicked: {
                switch (modelName) {
                    case "restore_default_btn":
                        settingsPage.modelRef.loadSettings(true)
                        dialog.setData(qsTr("Success"),qsTr("Settings has been succesfully restored to default."));
                        break;
                    case "clear_cache_btn":
                        localStorage.clearStorage();
                        savedTimetables.modelRef.clear()
                        dialog.setData(qsTr("Success"),qsTr("Local storage has been succesfully cleared"));
                        break;
                }
            }
        }
    }

    ListView {
        anchors.fill: parent
        model: SettingsGroupModel {
            id: settingsModel
        }
        delegate: Rectangle {
            width: parent.width
            height: appSettings.rowHeight
            color: appSettings.componentColor
            BottomBorder {
            }
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: appSettings.margin
                anchors.rightMargin: appSettings.margin
                StyledText {
                    text: name
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
                Loader {
                    id: loaderDelegate
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    width: appSettings.rowHeight * 0.75
                    height: width
                    property bool initialized: false
                    property var modelData: model.value
                    property var modelName: model.name
                    Component.onCompleted: {
                        switch (type)  {
                        case "slider":
                            sourceComponent = switchDelegate;
                            break;
                        case "color":
                            sourceComponent = colorDelegate;
                            break;
                        case "number":
                            sourceComponent = numberDelegate;
                            break;
                        case "list":
                            sourceComponent = listDelegate;
                            break;
                        case "btn":
                            sourceComponent = btnDelegate;
                            break;
                        }
                    }
                    onModelDataChanged: {
                        if (initialized) {
                            settingsPage.modelRef.updateSetting(settingsModel.getGroup(),
                                                                name,modelData)
                        }
                        initialized = true
                    }
                }
            }
        }
    }
}
