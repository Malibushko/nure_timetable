import QtQuick 2.12
import QtQuick.Controls 2.12

import "../styles"

Dialog {
    id: root_
    anchors.centerIn: overlay
    width: styles.rowWidth*0.75
    height: root.height * 0.6
    modal: true

    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape | Popup.CloseOnPressOutside

    readonly property var property_names: [qsTr("Date"),qsTr("Subject"),qsTr("Type"),
        qsTr("Groups"),qsTr("Auditory"),qsTr("Time start"),
        qsTr("Time end")]
    property var properties: [0,1,2,3,4,5,6]

    header: Rectangle {
        height: parent.height * 0.1
        StyledText {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: styles.margin
            color: styles.iconColor
            text: properties[1]
            font.bold: true
        }

        color: styles.themeColor
    }
    contentItem: ListView {
        model: property_names.length
        clip: true
        delegate: Rectangle {
            height: styles.rowHeight
            anchors.right: parent.right
            anchors.left: parent.left
            color: "transparent"
            Column {
                spacing: parent.height/4
                StyledText {
                    color: styles.utilityColor
                    text: property_names[index]
                }
                StyledText {
                    color: styles.textColor
                    text: properties[index]
                }
            }
        }
    }
}
