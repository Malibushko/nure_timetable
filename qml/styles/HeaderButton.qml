import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    icon.height: styles.iconSize
    icon.width: styles.iconSize
    icon.color: styles.iconColor
    scale: styles.iconScaleDefault

    visible: mainView.currentItem !== this
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
