import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    icon.height: styles.rowHeight/2
    icon.width: styles.rowHeight/2
    icon.color: styles.iconColor

    visible: mainView.currentItem !== this
    ToolTip.visible: pressed
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    hoverEnabled: true
    background: Rectangle {
        color: "transparent"
    }
    onPressed: {
        scale = 1.2
    }
    onReleased: {
        scale = 1
    }
}
