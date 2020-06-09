import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import lib 1.0
import "../styles"
Page {
    id: root_
    property alias modelRef: tableModel
    onWidthChanged: {
        tableView.forceLayout()
        rowsHeader.forceLayout();
        columnsHeader.forceLayout();
    }
    onHeightChanged: {
        tableView.forceLayout()
        rowsHeader.forceLayout();
        columnsHeader.forceLayout();
    }
    function resetOffset() {
        tableView.contentX = 0;
    }

    TableView {
        id: tableView
        anchors.fill: parent
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        rowSpacing: 1
        columnSpacing: 1
        bottomMargin: columnsHeader.height
        rowHeightProvider: function(row) {return (height-bottomMargin-(columnSpacing*rows-1))/rows;}
        columnWidthProvider: function(column) {return width/4;}

        model: TableModel {
            id: tableModel
            // @disable-check M16
            onHorizontalHeaderFinished: {
                columnsHeaderModel.model = tableModel.horizontalHeaderData()
                console.log(tableView.columnWidthProvider(1),tableModel.currentColumn())
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
                mainView.push(timetable)
            }
        }
        delegate: Rectangle {
            id: tableDelegate
            color: model.color ? model.color : "transparent"
            border.color: Material.color(Material.LightGreen)
            border.width: 1
            Text {
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
            Repeater {
                id: columnsHeaderModel
                Label {
                    width: tableView.columnWidthProvider(1)
                    height: 35
                    Text {
                        text: modelData
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 15
                        padding: 10
                    }
                    verticalAlignment: Text.AlignHCenter | Text.AlignVCenter
                    background: Rectangle { color: Material.color(Material.Green) }
                }
            }
        }

        Column {
            id: rowsHeader
            x: tableView.contentX
            spacing: tableView.rowSpacing
            anchors.top: parent.top
            z: 2
            Repeater {
                id: rowsHeaderModel
                Label {
                    width: Math.max(tableView.columnWidthProvider(1)/4,text.width*1.5)
                    height: Math.max(tableView.rowHeightProvider(1),text.height*1.5)
                    StyledText {
                        id: text
                        text: modelData
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 15
                    }
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle { color: Material.color(Material.Green)  }
                }
            }
        }
    }
}
