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
    property bool isTeacher: false
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
                listModel_.setFilterString(searchInput.text)
            }
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: SearchFilterModel {
                id: listModel_
            }
            delegate: Rectangle {
                width: appSettings.rowWidth
                color: appSettings.componentColor
                height: appSettings.rowHeight
                BottomBorder {
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: appSettings.animationsEnabled
                    onHoveredChanged: {
                        parent.color = containsMouse ? Qt.darker(appSettings.componentColor,1.05)
                                                     : appSettings.componentColor;
                        parent.border.color = containsMouse ? appSettings.accentColor
                                                            : "transparent"
                        parent.border.width = containsMouse
                    }
                    onClicked: {
                        api.schedule(timetable_id,isTeacher);
                        findPage.timetableId = model.timetable_id;
                        findPage.timetableTitle = model.title
                        findPage.isTeacher = isTeacher;
                    }
                }
                RowLayout {
                    anchors.fill: parent
                    StyledText {
                        Layout.leftMargin: appSettings.margin
                        Layout.alignment: Qt.AlignLeft
                        text: title
                    }
                }
            }
        }
    }
}
