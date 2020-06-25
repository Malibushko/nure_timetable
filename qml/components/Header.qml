import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import "../styles"

ToolBar {
    height: appSettings.rowHeight
    function updateIcons() {
        savedTimetableButton.visible = mainView.find(function(item,index){return item === savedTimetables;}) === null
    }

    RowLayout {
        anchors.fill: parent
        RowLayout {
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: appSettings.margin
            HeaderButton {
                id: backButton
                visible: mainView.depth > 1
                Layout.alignment: Qt.AlignLeft
                icon.source: "qrc:///qml/icons/back.svg"
                ToolTip.text: qsTr("Return to the previous page")
                onClicked: {
                    mainView.pop()
                }
            }
            HeaderButton {
                id: settingsButton
                visible: !backButton.visible
                Layout.alignment: Qt.AlignLeft
                icon.source: "qrc:///qml/icons/settings.svg"
                ToolTip.text: qsTr("Application settings")
                onClicked: {
                    mainView.push(settingsPage)
                }
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: appSettings.margin
            spacing: appSettings.margin
            HeaderButton {
                id: savedTimetableButton
                visible: mainView.currentItem !== savedTimetables
                icon.source: "qrc:///qml/icons/calendar.svg"
                ToolTip.text: qsTr("Saved timetables")
                onClicked: {
                    mainView.push(savedTimetables)
                }
            }
            HeaderButton {
                id: findButton
                visible: mainView.currentItem !== findPage
                icon.source: "qrc:///qml/icons/search.svg"
                ToolTip.text: qsTr("Search for a timetable")
                onClicked: {
                    mainView.push(findPage)
                }
            }
        }
    }
}
