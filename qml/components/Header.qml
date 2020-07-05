import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import "../styles"

ToolBar {
    id: root_
    height: Math.min(styles.rowHeight,Math.max(styles.iconSize+styles.margin/2,styles.rowHeight*0.75))

    RowLayout {
        anchors.fill: parent
        RowLayout {
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: styles.margin
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
            Layout.rightMargin: styles.margin
            spacing: styles.margin
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
