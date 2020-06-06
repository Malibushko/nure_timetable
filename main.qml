import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import "./qml/components"

ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("TimeTable")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    Settings {
        id: appSettings
    }
    header: Header {}

    StackView {
        id: mainView
        anchors.fill: parent
        initialItem: SavedTimetables {
            id: savedTimetables
        }
        FindPage {
            id: findPage
        }
    }
}
