import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Button {
    icon.height: styles.iconSize
    icon.width: styles.iconSize
    icon.color: styles.iconRowButtonColor
    scale: styles.iconScaleDefault

    ToolTip.visible: pressed
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval

    hoverEnabled: true

    onHoveredChanged: {
        icon.color = hovered ? styles.accentColor : styles.iconRowButtonColor
    }

    background: Rectangle {
        color: "transparent"
    }
}
