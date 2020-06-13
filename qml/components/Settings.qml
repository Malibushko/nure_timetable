import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12

Item {
    property bool isPortrait: root.width >= root.height
    property var appTheme:  (settingsPage.modelRef.value("styling","night_mode") === "true"?
                                Material.Dark : Material.Light)
    property bool isLight: (appTheme === Material.Light)
    property int screenWidth: root.width
    property int screenHeight: root.height
    property double margin: (isPortrait ? screenWidth : screenHeight) * 0.05
    property double padding: (isPortrait ? screenWidth : screenHeight) * 0.05
    property double rowHeight: screenHeight * 0.1
    property double rowWidth: screenWidth

    property color darkLight: Qt.darker("white",1.2);

    property color iconColor: (isLight ? "white" : darkLight)
    property color iconRowButtonColor: (isLight ? Material.color(Material.Grey) : darkLight)
    property color themeColor: (isLight? settingsPage.modelRef.value("styling","app_theme")
                                       : Material.backgroundColor)
    property color accentColor: Qt.darker(settingsPage.modelRef.value("styling","app_accent"),
                                          isLight ? 1.5 : 1)
    property color primaryColor: (isLight? settingsPage.modelRef.value("styling","app_primary")
                                        : Material.backgroundColor)
    property color utilityColor: Material.color(Material.Grey)
    property color componentColor: (isLight ? "white" : Material.backgroundColor)
    property color textColor: (isLight ?  "black" : darkLight)
}
