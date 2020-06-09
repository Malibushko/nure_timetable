import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import lib 1.0

Page {
    id: root_
    property alias modelRef: tableModel
    TableView {
        id: tableView
        anchors.fill: parent
        clip: true
        rowSpacing: 1
        columnSpacing: 1
        topMargin: columnsHeader.height
        rowHeightProvider: function(row) {return height/rows;}
        columnWidthProvider: function(column) {return width/4;}

        model: TableModel {
            id: tableModel
            onHorizontalHeaderFinished: {
                columnsHeaderModel.model = tableModel.horizontalHeaderData()
                tableView.forceLayout()
            }
            onVerticalHeaderFinished: {
                rowsHeaderModel.model = tableModel.verticalHeaderData()
                tableView.forceLayout()
            }
        }
        delegate: Rectangle {
            id: tableDelegate
            border.width: 1
            Text {
                anchors.centerIn: parent
                text: index
            }
        }
        Row {
            id: columnsHeader
            y: tableView.contentY
            x: tableView.x
            z: 2
            spacing: tableView.columnSpacing
            Repeater {
                id: columnsHeaderModel
                Label {
                    width: tableView.columnWidthProvider(1)
                    height: 35
                    text: modelData
                    color: '#aaaaaa'
                    font.pixelSize: 15
                    padding: 10
                    verticalAlignment: Text.AlignVCenter

                    background: Rectangle { color: "#333333" }
                }
            }
        }
        Column {
            id: rowsHeader
            x: tableView.contentX
            spacing: tableView.rowSpacing
            z: 2
            Repeater {
                id: rowsHeaderModel
                Label {
                    width: 180
                    height: tableView.rowHeightProvider(1)
                    text: modelData
                    color: '#aaaaaa'
                    font.pixelSize: 15
                    padding: 10
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle { color: "#333333" }
                }
            }
        }
    }
}
