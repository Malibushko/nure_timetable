import QtQuick 2.15
import QtQuick.Controls 2.15

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
    function imitateClick() {
        clicked()
    }

    background: Rectangle {
        color: "transparent"
    }
}
