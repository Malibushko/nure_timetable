import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    icon.height: styles.iconSize
    icon.width: styles.iconSize
    icon.color: styles.iconColor
    scale: styles.iconScaleDefault

    ToolTip.visible: pressed
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    hoverEnabled: true
    background: Rectangle {
        color: "transparent"
    }
    onPressed: {
        scale = styles.iconScalePressed
    }
    onReleased: {
        scale = styles.iconScaleDefault
    }
}
