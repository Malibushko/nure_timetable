import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import "../styles"
import lib 1.0

Page {
    id: root_
    property var currentGroup: -1
    ListView {
        id: mainList
        anchors.fill: parent
        model: mainSettings.groups()
        delegate: Rectangle {
            width: root_.width
            height: styles.rowHeight
            color: hoverCatch.containsMouse ? Qt.darker(styles.componentColor,1.02) : styles.componentColor
            BottomBorder {
            }
            MouseArea {
                id: hoverCatch
                anchors.fill: parent
                hoverEnabled: styles.animationsEnabled
                onHoveredChanged: {
                    parent.border.color = containsMouse ? styles.accentColor : "transparent"
                    parent.border.width = containsMouse
                }
                onClicked: {
                    root_.currentGroup = mainList.model[index];
                    settingsGroup.modelRef.setItems(mainSettings.groupKeys(mainList.model[index]))
                    mainView.push(settingsGroup);
                }
            }
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: styles.margin
                anchors.rightMargin: styles.margin
                StyledText {
                    text: mainSettings.stringify(mainList.model[index])
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
                RowButton {
                    Layout.alignment: Qt.AlignRight
                    icon.source: "qrc:///qml/icons/back.svg"
                    icon.color: styles.iconRowButtonColor
                    rotation: 180
                }
            }
        }
    }
}
