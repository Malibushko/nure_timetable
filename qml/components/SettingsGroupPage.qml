import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
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
                    Component.onCompleted: {
                        if (type == "slider")
                            sourceComponent = switchDelegate;
                        else if (type == "color")
                            sourceComponent = colorDelegate
                        else if (type == "number")
                            sourceComponent = numberDelegate;
                        else if (type == "list")
                            sourceComponent = listDelegate
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
