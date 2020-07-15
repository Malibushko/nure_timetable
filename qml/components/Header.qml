import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.0
import "../styles"
import lib 1.0

ToolBar {
    id: root_
    height: styles.rowHeight*0.75
    function backClick() {
        mainView.pop()
    }
    function setTimer(time) {
        countdownTimer.value = time;
        countdownTimer.restart();
    }
    RowLayout {
        anchors.fill: parent

        HeaderButton {
            id: backButton
            visible: mainView.depth > 1
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.leftMargin: styles.margin
            icon.source: "qrc:///qml/icons/back.svg"
            ToolTip.text: qsTr("Return to the previous page")
            onClicked: backClick()
        }
        HeaderButton {
            id: settingsButton
            visible: !backButton.visible
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.leftMargin: styles.margin
            icon.source: "qrc:///qml/icons/settings.svg"
            ToolTip.text: qsTr("Application settings")
            onClicked: {
                mainView.push(settingsPage)
            }
        }
        HeaderButton {
            id: timetableTimer
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: height*2
            Layout.rightMargin: styles.margin
            Layout.leftMargin: styles.margin
            visible: mainView.currentItem === timetablePage

            FontLoader {
                id: digitalFont
                source: "qrc:///qml/fonts/digital.ttf"
            }

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
                id: countdownTimer
                running: value !== 0
                triggeredOnStart: true
                repeat: true
                onTriggered: {
                    timeDisplay.text = timetablePage.modelRef.secondsToString(--value);
                }
            }
            Text {
                id: timeDisplay
                visible: countdownTimer.running
                anchors.centerIn: parent
                width: parent.width*0.75
                fontSizeMode: Text.Fit
                font.pixelSize: parent.height*0.5
                font.family:  digitalFont.name
                color: styles.iconColor
            }
            ToolTip.text: qsTr("Timer shows how much time left till the end of lesson")
        }

        HeaderButton {
            id: findButton
            visible: mainView.depth === 1
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.rightMargin: styles.margin
            icon.source: "qrc:///qml/icons/search.svg"
            ToolTip.text: qsTr("Search for a timetable")
            onClicked: {
                mainView.push(findPage)
            }
        }
        HeaderButton {
            id: saveButton
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.rightMargin: styles.margin
            visible: mainView.currentItem === timetablePage
            enabled: isSaved(timetablePage.modelRef.id())

            onVisibleChanged: {
                if (visible)
                    enabled = isSaved(timetablePage.modelRef.id())
            }
            function isSaved(id) {
                console.log("ID:" + id)
                return savedTimetables.modelRef.find(id) < 0
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
                                                             timetablePage.modelRef.getModel());
                localStorage.save(TableType.SAVED_TIMETABLE,timetable)
                savedTimetables.modelRef.addItem(timetable);
            }
        }
    }
}
