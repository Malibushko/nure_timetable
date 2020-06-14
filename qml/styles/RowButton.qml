import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Button {
    icon.height: appSettings.rowHeight/2
    icon.width: appSettings.rowHeight/2
    icon.color: appSettings.iconRowButtonColor
    ToolTip.visible: pressed
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval

    hoverEnabled: true

    onHoveredChanged: {
        icon.color = hovered ? appSettings.accentColor : appSettings.iconRowButtonColor
    }

    background: Rectangle {
        color: "transparent"
    }
}
