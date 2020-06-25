import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import lib 1.0
import "../styles"
Page {
    id: root_
    property alias modelRef: tableModel

    Component {
        id: headerDelegate
        Label {
            Text {
                text: tableView.horizontalHeader[column_]
                anchors.centerIn: parent
                color: (!appSettings.isLight ? appSettings.darkLight : "white")
            }
            verticalAlignment: Text.AlignHCenter | Text.AlignVCenter
            background: Rectangle { color: appSettings.themeColor }
        }
    }
    Component {
        id: cellDelegate
        Rectangle {
            color: (color_ && appSettings.isLight) ? color_ : "transparent"
            border.color: appSettings.accentColor
            border.width: 1
            StyledText {
                anchors.centerIn: parent
                text: auditory + "\n" + subject + "\n" + type;
                clip: true
                fontSizeMode: Text.Fit
            }
        }
    }

    HeaderButton {
        parent: mainHeader
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: appSettings.margin
        visible: mainView.currentItem === timetablePage && savedTimetables.modelRef.find(findPage.timetableId) < 0
        background: Rectangle {
            anchors.fill: parent
            color: mainHeader.background.color
        }
        icon.source: "qrc:///qml/icons/save"
        ToolTip.text: qsTr("Save timetable")
        Component.onCompleted: {
            console.log(findPage.timetableId,savedTimetables.modelRef.find(findPage.timetableId))
        }

        onClicked: {

            dialog.setData(qsTr("Success"),qsTr("Timetable saved!"));

            var timetable = localStorage.createTimetable(findPage.timetableId,
                                                         findPage.timetableTitle,
                                                         findPage.isTeacher,
                                                         tableModel.getModel());
            localStorage.save(TableType.SAVED_TIMETABLE,timetable)
            savedTimetables.modelRef.addItem(timetable);
        }
    }

    Column {
        id: rowsHeader
        spacing: tableView.rowSpacing
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        z: 2
        Repeater {
            id: rowsHeaderModel
            model: tableView.verticalHeader
            Label {
                width: text.width*1.5
                height: tableView.rowHeightProvider(index)
                onHeightChanged: {
                    fillAnimationAnimation.restart()
                }
                StyledText {
                    id: text
                    text: modelData
                    anchors.centerIn: parent
                    color: (!appSettings.isLight ? appSettings.darkLight : "white")
                }
                BottomBorder {
                    color: appSettings.accentColor
                }
                Rectangle {
                    id: fillAnimationRect
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    PropertyAnimation {
                        id: fillAnimationAnimation
                        target: fillAnimationRect
                        property: "height"
                        from: (tableModel.rowProgress(index)/tableModel.lessonDuration())*tableView.rowHeightProvider(index)
                        to: from === 0 ? 0 : tableView.rowHeightProvider(index)
                        duration: tableModel.rowProgress(index)*1000
                        running: true
                        Component.onCompleted: {
                        }
                    }
                    color: appSettings.accentColor
                    opacity: 0.5
                }
                verticalAlignment: Text.AlignVCenter
                background: Rectangle { color: appSettings.themeColor  }
            }
        }
    }

    TableView {
        id: tableView
        width: parent.width
        height: parent.height
        boundsBehavior: Flickable.StopAtBounds
        columnSpacing: 1
        property var horizontalHeader
        property var verticalHeader
        property var columnsPerScreen: 4
        rowHeightProvider: function (row) {
            if (row === rows-1)
                return 50;
            return (height-rowHeightProvider(rows-1))/(rows-1)
        }
        columnWidthProvider: function(column) {
            return width/columnsPerScreen;
        }
        onWidthChanged: {
            forceLayout()
        }
        onHeightChanged: {
            contentHeight = height
            forceLayout()
        }

        model: TableModel {
            id: tableModel
            onHorizontalHeaderFinished: {
                tableView.contentX = (tableView.columnWidthProvider(1)-0.5) * tableModel.currentColumn()
                tableView.horizontalHeader = tableModel.horizontalHeaderData()
            }
            onVerticalHeaderFinished: {
                tableView.verticalHeader = tableModel.verticalHeaderData()
            }
            onTimetableCompleted: {
                mainView.push(timetablePage)
            }
        }
        delegate: Loader {
            id: loader
            property var auditory: model.auditory
            property var subject: model.subject
            property var type: model.type
            property var color_
            property var row_: row
            property var column_: column
            sourceComponent: (row == tableView.rows - 1 ? headerDelegate : cellDelegate)
            onTypeChanged: {
                switch (model.type) {
                case "Пз":
                    color_ =  settingsPage.modelRef.value("timetable_styling","pz_color");
                    break;
                case "Лк":
                    color_ =  settingsPage.modelRef.value("timetable_styling","lc_color");
                    break;
                case "Лб":
                    color_ =  settingsPage.modelRef.value("timetable_styling","lb_color");
                    break;
                case "Зал":
                    color_ =  settingsPage.modelRef.value("timetable_styling","zal_color");
                    break;
                default:
                    color_ = "transparent";
                }
            }
        }
    }
}
