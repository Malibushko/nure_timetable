import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import lib 1.0
import "../styles"
Page {
    id: root_
    property alias modelRef: tableModel
    function resetOffset() {
        tableView.contentX = 0;
    }
    HeaderButton {
        parent: mainHeader
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: appSettings.margin
        visible: mainView.currentItem == timetablePage
        background: Rectangle {
            anchors.fill: parent
            color: mainHeader.background.color
        }
        icon.source: "qrc:///qml/icons/save"
        ToolTip.text: qsTr("Save timetable")
        onClicked: {
            var timetable = localStorage.createTimetable(findPage.timetableId,
                                                         findPage.timetableTitle,
                                                         findPage.isTeacher,
                                                         tableModel.getModel());
            localStorage.save(TableType.SAVED_TIMETABLE,timetable)
            savedTimetables.modelRef.addItem(timetable);
        }
    }

    TableView {
        id: tableView
        anchors.fill: parent
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        rowSpacing: 1
        columnSpacing: 1
        bottomMargin: columnsHeader.height
        property var cell_height : (height-bottomMargin-(columnSpacing*rows-1))/rows
        property var cell_width: width/4
        rowHeightProvider: function(row) {return 180;}
        columnWidthProvider: function(column) {return 200;}
        onWidthChanged: {
            forceLayout();
        }
        onHeightChanged: {
            forceLayout();
        }

        model: TableModel {
            id: tableModel
            // @disable-check M16
            onHorizontalHeaderFinished: {
                columnsHeaderModel.model = tableModel.horizontalHeaderData()
                tableView.contentX = (tableView.columnWidthProvider(1)-0.5) * tableModel.currentColumn()
                tableView.forceLayout()
            }
            // @disable-check M16
            onVerticalHeaderFinished: {
                rowsHeaderModel.model = tableModel.verticalHeaderData()
                tableView.forceLayout()
            }
            // @disable-check M16
            onTimetableCompleted: {
                mainView.push(timetablePage)
            }
        }
        delegate: Rectangle {
            id: tableDelegate
            color: (model.color && appSettings.isLight) ? model.color : "transparent"
            border.color: appSettings.accentColor
            border.width: 1
            StyledText {
                anchors.centerIn: parent
                text: auditory + "\n" + subject + "\n" + type
            }
        }
        Row {
            id: columnsHeader
            y: tableView.contentY
            z: 2
            spacing: tableView.columnSpacing
            anchors.top: parent.bottom
            onWidthChanged: {
                forceLayout();
            }
            onHeightChanged: {
                forceLayout();
            }

            Repeater {
                id: columnsHeaderModel
                Label {
                    width: tableView.columnWidthProvider(1)
                    height: 35
                    Text {
                        text: modelData
                        anchors.centerIn: parent
                        color: (!appSettings.isLight ? appSettings.darkLight : "white")
                    }
                    verticalAlignment: Text.AlignHCenter | Text.AlignVCenter
                    background: Rectangle { color: appSettings.themeColor }
                }
            }
        }

        Column {
            id: rowsHeader
            x: tableView.contentX
            spacing: tableView.rowSpacing
            anchors.top: parent.top
            z: 2
            onWidthChanged: {
                forceLayout();
            }
            onHeightChanged: {
                forceLayout();
            }

            Repeater {
                id: rowsHeaderModel
                Label {
                    width: Math.max(tableView.columnWidthProvider(1)/4,text.width*1.5)
                    height: Math.max(tableView.rowHeightProvider(1),text.height*1.5)
                    StyledText {
                        id: text
                        text: modelData
                        anchors.centerIn: parent
                        color: (!appSettings.isLight ? appSettings.darkLight : "white")
                    }
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle { color: appSettings.themeColor  }
                }
            }
        }
    }
}
