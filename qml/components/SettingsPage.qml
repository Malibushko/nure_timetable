import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import "../styles"
import lib 1.0
import Qt.labs.qmlmodels 1.0

Page {
    id: root_
    property var currentGroup: -1
    ListView {
        id: mainList
        anchors.fill: parent
        model: mainSettings.groups()
        delegate: Rectangle {
            width: parent.width
            height: appSettings.rowHeight
            color: appSettings.componentColor
            BottomBorder {
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: appSettings.animationsEnabled
                onHoveredChanged: {
                    parent.color = containsMouse ? Qt.darker(appSettings.componentColor,1.05) :
                                                   appSettings.componentColor;
                    parent.border.color = containsMouse ? appSettings.accentColor : "transparent"
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
                anchors.leftMargin: appSettings.margin
                anchors.rightMargin: appSettings.margin
                StyledText {
                    text: mainSettings.stringify(mainList.model[index])
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
            }
        }
    }
}
