import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    icon.height: appSettings.rowHeight/2
    icon.width: appSettings.rowHeight/2
    icon.color: appSettings.iconColor

    visible: mainView.currentItem !== this
    ToolTip.visible: pressed
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    hoverEnabled: true
    background: Rectangle {
        color: "transparent"
    }
}
