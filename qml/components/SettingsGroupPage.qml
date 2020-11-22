import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "../styles"
import lib 1.0

Page {
    property alias modelRef: settingsModel
    id: root_
    Component {
        id: switchDelegate
        Switch {
            id: innerItem
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Component.onCompleted: {
                checked =  modelData
            }
            onCheckedChanged: {
                modelData = checked
            }
            function click() {
                innerItem.checked = !innerItem.checked
            }
        }
    }
    /*
      There is a bug in Qt now (05.07.2020)
      ColorDialog may not work on some platform because
      of missing file (DefaultWindowDecoration.qml) in source
      To fix this look for this in on Internet (it`s easy) and
      add it to your sources to the following path:
      ~/Qt/5.15.0/android/qml/QtQuick/Dialogs/
      Everything should work fine then
     */
    Component {
        id: colorDelegate
        Rectangle {
            radius: width
            color: modelData
            MouseArea {
                id: clickCatcher
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
            function click() {
                colorPicker.open()
            }
        }
    }
    Component {
        id: numberDelegate
        SpinBox {
            from: 1
            to: 10
            value: modelData
            anchors.margins: styles.margin
        }

    }
    Component {
        id: listDelegate
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            ComboBox {
                clip: true
                model: modelData.split(';')
                anchors.fill: parent
                down: true
                displayText: lang.getLanguage()
                onActivated:  {
                    console.log(currentText)
                    mainSettings.update(-1,
                                        SETTINGS_TYPE.CHOSEN_LANGUAGE,
                                        currentText)
                    lang.setLanguage(currentText);
                    displayText = currentText
                }
            }
        }
    }
    Component {
        id: btnDelegate
        RowButton {
            id: pressButton
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: styles.margin
            icon.source: "qrc:///qml/icons/security.svg"
            onClicked: {
                switch (modelName) {
                    case SETTINGS_TYPE.CLEAR_CACHE_BTN:
                        mainSettings.load(true)
                        dialog.setData(qsTr("Success"),
                                       qsTr("Settings has been succesfully restored to default.
                                             Restart the app to come them into force"));
                        break;
                    case SETTINGS_TYPE.RESTORE_DEFAULT_BTN:
                        localStorage.clearStorage();
                        savedTimetables.modelRef.clear();
                        dialog.setData(qsTr("Success"),qsTr("Local storage has been succesfully cleared"));
                        break;
                }
            }
            function click() {
                clicked()
            }
        }
    }

    ListView {
        anchors.fill: parent
        model: SettingsGroupModel {
            id: settingsModel
        }
        delegate: Rectangle {
            width: root_.width
            height: styles.rowHeight
            color: hoverCatch.containsMouse ? Qt.darker(styles.componentColor,1.02) : styles.componentColor
            BottomBorder {}
            MouseArea {
                id: hoverCatch
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    console.log("Trying to send click")
                    loaderDelegate.item.click()
                }
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: styles.margin
                anchors.rightMargin: styles.margin
                StyledText {
                    id: settingsName
                    text: mainSettings.stringify(name)
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
                Loader {
                    id: loaderDelegate
                    signal clicked()
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.preferredWidth: styles.rowHeight * 0.75
                    Layout.preferredHeight: styles.rowHeight * 0.75

                    property bool initialized: false
                    property var modelData: model.value
                    property var modelName: model.name

                    Component.onCompleted: {
                        switch (type)  {
                        case CONTROL.SWITCH:
                            sourceComponent = switchDelegate;
                            break;
                        case CONTROL.COLOR:
                            Layout.preferredWidth = styles.iconSize
                            Layout.preferredHeight = styles.iconSize
                            sourceComponent = colorDelegate;
                            break;
                        case CONTROL.NUMBER:
                            Layout.preferredWidth = styles.rowHeight * 2
                            sourceComponent = numberDelegate;
                            break;
                        case CONTROL.LIST:
                            Layout.preferredWidth = styles.rowWidth*0.33
                            sourceComponent = listDelegate;
                            break;
                        case CONTROL.BTN:
                            sourceComponent = btnDelegate;
                            break;
                        }
                    }
                    onModelDataChanged: {
                        if (initialized) {
                            mainSettings.update(settingsPage.currentGroup,name,modelData)
                        }
                        initialized = true
                    }
                }
            }
        }
    }
}
