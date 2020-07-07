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
            leftPadding: styles.margin
            bottomPadding: 0
            Layout.preferredHeight: styles.rowHeight
            // to fix QtBug when text changes to empty for some reasons
            inputMethodHints: Qt.ImhNoPredictiveText |  Qt.ImhSensitiveData
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            onTextChanged: {
                    listModel_.setFilterString(searchInput.text)
            }
        }
        ListView {
            id: list
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            model: SearchFilterModel {
                id: listModel_
            }
            delegate: Rectangle {
                width: styles.rowWidth
                color: styles.componentColor
                height: styles.rowHeight
                BottomBorder {
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: styles.animationsEnabled
                    onHoveredChanged: {
                        parent.color = containsMouse ? Qt.darker(styles.componentColor,1.05)
                                                     : styles.componentColor;
                        parent.border.color = containsMouse ? styles.accentColor
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
                        Layout.leftMargin: styles.margin
                        Layout.alignment: Qt.AlignLeft
                        text: title
                    }
                }
            }
        }
    }
}
