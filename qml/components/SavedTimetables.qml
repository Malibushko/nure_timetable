import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import "../styles"

Page {
    id: root_
    ListView {
        clip: true
        boundsBehavior: Flickable.OvershootBounds
        anchors.fill: parent
        model: ListModel {
            ListElement {
                timetable_id: 1
                title: "Timetable"
                last_update: "06/06/2020"
            }
            ListElement {
                timetable_id: 1
                title: "Timetable"
                last_update: "06/06/2020"
            }
            ListElement {
                timetable_id: 1
                title: "Timetable"
                last_update: "06/06/2020"
            }
        }

        delegate: Rectangle {
            width: parent.width
            height: appSettings.rowHeight

            RowLayout {
                anchors.fill: parent
                ColumnLayout {
                    Layout.preferredWidth: parent.width/2
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: appSettings.margin
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(title)
                    }
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(last_update)
                        color: Material.color(Material.Grey)
                    }
                }
                RowLayout {
                    Layout.preferredWidth: appSettings.rowHeight * 3
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: appSettings.margin
                    RowButton {
                        id: updateButton
                        Layout.alignment: Qt.AlignRight
                        icon.source: "qrc:///qml/icons/refresh.svg"
                        ToolTip.text: qsTr("Update timetable")
                    }
                    RowButton {
                        id: removeButton
                        Layout.alignment: Qt.AlignRight
                        icon.source: "qrc:///qml/icons/trash.svg"
                        ToolTip.text: qsTr("Remove timetable")
                    }
                }
            }
            BottomBorder {
                color: Material.color(Material.LightGreen)
            }
        }
    }
}
