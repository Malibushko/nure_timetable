import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import lib 1.0
import "../styles"
Page {
    id: root_
    property alias modelRef: tableModel
    FontLoader {
        id: digitalFont; source: "https://ff.static.1001fonts.net/d/i/digital-7.regular.ttf"
    }

    Component {
        id: headerDelegate
        Label {
            Text {
                text: tableView.horizontalHeader[column_]
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
            color: (color_ && styles.isLight) ? color_ : "transparent"
            border.color: styles.accentColor
            border.width: 1
            StyledText {
                anchors.fill: parent
                StyledText {
                    anchors.centerIn: parent
                    text: auditory + "\n" + subject + "\n" + type;
                }
                clip: true
                fontSizeMode: Text.Fit
            }
        }
    }
    HeaderButton {
        id: timetableTimer
        anchors.centerIn: parent
        parent: mainHeader
        height: parent.height
        width: height*2
        visible: mainView.currentItem == root_
        Connections {
            target: mainSettings
            function onValueChanged(group,key,value) {
                if (group === SETTINGS_GROUP.TIMETABLE_STYLING &&
                        key === SETTINGS_TYPE.SHOW_TIMER) {
                       timetableTimer.visible = mainView.currentItem == root_ && value;
                }
            }
        }

        Timer {
            property int value
            // small hack to start timer immidiately and then set interval to 1 second
            property bool isFirst
            running: value !== 0
            id: countdownTimer
            interval: 1
            onTriggered: {
                if (isFirst) {
                    interval = 1000;
                    isFirst = false;
                }
                timeDisplay.text = tableModel.secondsToString(--value);
            }
            function setTime(count) {
                    value = count;
                    isFirst = true
                    countdownTimer.start();
            }
        }
        Text {
            id: timeDisplay
            visible: countdownTimer.running
            anchors.centerIn: parent
            width: parent.width*0.75
            fontSizeMode: Text.Fit
            font.pixelSize: parent.height*0.5
            font.family:  digitalFont.status === FontLoader.Ready ?  digitalFont.name : "Arial"
            color: styles.iconColor
        }
        ToolTip.text: qsTr("Timer shows how much time left till the end of lesson")
    }

    HeaderButton {
        parent: mainHeader
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: styles.margin
        enabled: true
        visible: mainView.currentItem === timetablePage && savedTimetables.modelRef.find(tableModel.id()) < 0
        onVisibleChanged: {
            enabled = true
        }

        background: Rectangle {
            anchors.fill: parent
            color: mainHeader.background.color
        }
        icon.source: "qrc:///qml/icons/save.svg"
        icon.color: enabled ? styles.iconColor : mainHeader.background.color

        ToolTip.text: qsTr("Save timetable")
        onClicked: {
            enabled = false
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
                    color: (!styles.isLight ? styles.darkLight : "white")
                }
                BottomBorder {
                    color: styles.accentColor
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
                        duration: (tableModel.lessonDuration()-tableModel.rowProgress(index))*1000
                        running: true
                        onStarted: {
                             if (tableModel.rowProgress(index) > 0 && styles.showTimer) {
                                    countdownTimer.setTime(duration/1000);
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
        property var columnsPerScreen: 4

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
