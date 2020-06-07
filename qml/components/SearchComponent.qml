import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4

import "../styles"
import lib 1.0


Item {
    property alias modelRef: listModel_
    property alias textInputRef : searchInput
    ColumnLayout {
        anchors.fill: parent
        TextField {
            id: searchInput
            leftPadding: appSettings.margin
            bottomPadding: 5
            Layout.preferredHeight: appSettings.rowHeight
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            onTextEdited: {
                listModel_.setFilter(searchInput.text)
            }
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model:  BaseSearchModel {
                id: listModel_
            }
            delegate: Rectangle {
                width: parent.width
                height: appSettings.rowHeight * (model.visible ? 1 : 0)
                visible: model.visible
                BottomBorder {
                    color: Material.color(Material.LightGreen)
                }
                RowLayout {
                    anchors.fill: parent
                    StyledText {
                        Layout.leftMargin: appSettings.margin
                        Layout.alignment: Qt.AlignLeft
                        text: title
                    }
                    RowButton {
                        id: addButton
                        Layout.alignment: Qt.AlignRight
                        Layout.rightMargin: appSettings.margin
                        icon.source: "qrc:///qml/icons/plus.svg"
                        ToolTip.text: qsTr("Save timetable")
                    }
                }

            }
        }
    }
}
