import QtQuick 2.15
import QtQuick.Controls 2.15

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
        qsTr("Time end"),qsTr("Note")]
    property var properties: [0,1,2,3,4,5,6,7]
    property var index

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
        Button {
            id: editButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: styles.margin
            checkable: true
            icon.source: !checked ? "qrc:///qml/icons/edit.svg" : "qrc:///qml/icons/done.svg"
            icon.color: styles.iconColor
            width: parent.height
            height: width
            ToolTip.text: qsTr("Edit note")
            background: Rectangle {
                color: "transparent"
            }
        }
        color: styles.themeColor
    }
    contentItem: ListView {
        id: infoList
        model: property_names.length
        clip: true
        delegate: Rectangle {
            id: delegate
            height: {
                if (noteEdit.visible)
                    return styles.rowHeight + noteEdit.contentHeight
                else
                    return styles.rowHeight;
            }
            anchors.right: parent.right
            anchors.left: parent.left
            color: "transparent"
            Column {
                spacing: parent.height/4
                StyledText {
                    id: sectionName
                    color: styles.utilityColor
                    text: property_names[index]
                }
                StyledText {
                    visible: !noteEdit.visible
                    color: styles.textColor
                    text: properties[index]
                }
                TextArea {
                    id: noteEdit
                    anchors.top: sectionName.bottom
                    enabled: editButton.checked
                    inputMethodHints: Qt.ImhNoPredictiveText |  Qt.ImhSensitiveData
                    onEnabledChanged: {
                        if (enabled) {
                            infoList.positionViewAtEnd()
                            forceActiveFocus()
                        } else {
                            if (root_.index && visible) {
                               timetablePage.modelRef.setNote(root_.index,text)
                            }
                        }
                    }

                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    width: delegate.width
                    text: properties[index]
                    placeholderText: qsTr("Remember to take laptop for this lesson")
                    visible: index === 7
                    color: styles.textColor
                }
            }
        }
    }
}
