import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    property bool isPortrait: root.width >= root.height
    property int screenWidth: root.width
    property int screenHeight: root.height
    property double margin: (isPortrait ? screenWidth : screenHeight) * 0.05
    property double padding: (isPortrait ? screenWidth : screenHeight) * 0.05
    property double rowHeight: screenHeight * 0.1
    property double rowWidth: screenWidth
    property var iconColor: "white"
}
