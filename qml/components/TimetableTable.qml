import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import lib 1.0
import "../styles"
Page {
    id: root_
    property alias modelRef: tableModel
    function dateFormat(date) {
        return ('0' + date.getDate()).slice(-2) + '.'
                     + ('0' + (date.getMonth()+1)).slice(-2) + '.'
                     + date.getFullYear().toString().slice(-2);
    }
    function showSettings() {
        tableSettings.modelRef.setModel(modelRef.lessonTablePtr(),modelRef.hideTablePtr())
        tableSettings.modelRef.generate()
        tableSettings.open()
    }

    Component {
        id: headerDelegate
        Label {
            Text {
                text: dateFormat(tableView.horizontalHeader[column_])
                anchors.centerIn: parent
                color: (!styles.isLight ? styles.darkLight : "white")
            }
            verticalAlignment: Text.AlignHCenter | Text.AlignVCenter
            background: Rectangle { color: styles.themeColor }
        }
    }
    Component {
        id: cellDelegate
        Rectangle {
            anchors.fill: parent
            color: {
                if (!is_visible)
                    return "transparent";
                else if (color_ && styles.isLight)
                    return color_;
                return "transparent";
            }
            border.color: styles.accentColor
            border.width: has_note ? 4 : 1

            StyledText {
                anchors.fill: parent
                StyledText {
                    visible: is_visible
                    anchors.centerIn: parent
                    text: auditory + "\n" + subject + "\n" + type;
                }
                clip: true
                fontSizeMode: Text.Fit
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (subject != "" && is_visible) {
                        lessonInfo.properties = properties
                        console.log(model_index)
                        lessonInfo.index = model_index
                        lessonInfo.open()
                    }
                }
            }
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
            property int activeRect: -1
            model: tableView.verticalHeader
            Label {
                width: text.width*1.5
                height: tableView.rowHeightProvider(index)
                onHeightChanged: {
                    fillAnimationAnimation.recalculate()
                }
                StyledText {
                    id: text
                    text: modelRef.verticalHeaderDataToString(index,"hh:mm")
                    anchors.centerIn: parent
                    color: (!styles.isLight ? styles.darkLight : "white")
                }
                BottomBorder {
                    color: styles.accentColor
                }
                Rectangle {
                    id: fillAnimationRect
                    property alias fillAnimationRef: fillAnimationAnimation
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    PropertyAnimation {
                        property var appState: Qt.application.state

                        id: fillAnimationAnimation
                        target: fillAnimationRect
                        property: "height"
                        running: true

                        function isRunning(index) {
                            return tableModel.rowProgress(index) > 0 && styles.showTimer;
                        }
                        function recalculate() {
                            from =    (tableModel.rowProgress(index)/tableModel.lessonDuration())*
                                    tableView.rowHeightProvider(index);
                            duration = (tableModel.lessonDuration() - tableModel.rowProgress(index))*1000
                            to = from === 0 ? 0 : tableView.rowHeightProvider(index)

                            if (duration/1000 !== tableModel.lessonDuration()) {
                                mainHeader.setTimer(duration/1000)
                                restart()
                            }
                        }
                        onAppStateChanged: {
                            if (appState === Qt.ApplicationActive && isRunning(index)) {
                                recalculate()
                            }
                        }
                        Component.onCompleted: {
                            recalculate()
                        }
                        onStarted: {
                            if (isRunning(index)) {
                                rowsHeaderModel.activeRect = index
                            }
                        }
                    }
                    color: styles.accentColor
                    opacity: 0.5
                }
                verticalAlignment: Text.AlignVCenter
                background: Rectangle { color: styles.themeColor  }
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
        property var columnsPerScreen: {
            if (tableView.width > 1280)
                return 8;
            else
                return 4;
        }

        rowHeightProvider: function (row) {
            if (row === rows-1)
                return styles.rowHeight/2;
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
            onNoteChanged: {
                console.log("Saving note" + note);
                localStorage.save(TableType.NOTE,note);
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
            property var is_visible: color_ == "transparent" ? true : model.visible
            property var properties: [date,subject,type,groups,auditory,timeStart,timeEnd,model.note]
            property var model_index: index
            property bool has_note: model.note != ""
            sourceComponent: (row == tableView.rows - 1 ? headerDelegate : cellDelegate)
            onTypeChanged: {
                switch (model.type) {
                case "Пз":
                    color_ =  styles.pz_color
                    break;
                case "Лк":
                    color_ =  styles.lc_color
                    break;
                case "Лб":
                    color_ =  styles.lb_color
                    break;
                case "Зал":
                    color_ =  styles.zal_color
                    break;
                default:
                    color_ = "transparent";
                }
            }
        }
    }
}
