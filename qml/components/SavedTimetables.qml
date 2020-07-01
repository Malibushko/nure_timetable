import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import "../styles"
import lib 1.0

Page {
    id: root_
    property alias modelRef: saveModel
    function forceLayout() {
        saveModel.clear()
    }

    Transition {
        id: removeAnimation
                SequentialAnimation {
                    ColorAnimation {
                        property: "color"
                        to: styles.primaryColor
                        duration: 200
                    }
                    NumberAnimation {
                        property: "y";
                        to: -styles.rowHeight
                        duration: 300
                        easing.type: Easing.OutQuad
                    }
                }
    }
    Rectangle {
        anchors.centerIn: parent
        visible: mainListView.count === 0
        width: parent.width/2
        height: styles.rowHeight
        color: "transparent"
        StyledText {
            anchors.centerIn: parent
            color: styles.utilityColor
            text: qsTr("Saved timetables will be displayed here")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }
    ListView {
        id: mainListView
        clip: true
        boundsBehavior: Flickable.OvershootBounds
        anchors.fill: parent
        model: SaveModel {
            id: saveModel
            Component.onCompleted: {
                setItems(localStorage.get(TableType.SAVED_TIMETABLE))
            }
        }
        remove: styles.animationsEnabled ? removeAnimation : null

        delegate: Rectangle {
            width: parent.width
            height: styles.rowHeight
            color: styles.componentColor
            MouseArea {
                anchors.fill: parent
                hoverEnabled: styles.animationsEnabled
                onHoveredChanged: {
                    parent.color = containsMouse ? Qt.darker(styles.componentColor,1.05) : styles.componentColor;
                    parent.border.color = containsMouse ? styles.accentColor : "transparent"
                    parent.border.width = containsMouse
                }
                onClicked: {
                    var lessons = model.lessons;
                    timetablePage.modelRef.prepareForNewTimetable(lessons.length)
                    for (var i in lessons)
                        timetablePage.modelRef.addLesson(model.id,lessons[i])
                    mainView.push(timetablePage);
                }
            }

            RowLayout {
                anchors.fill: parent
                ColumnLayout {
                    Layout.preferredWidth: parent.width/2
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: styles.margin
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(title)
                        color: styles.textColor
                    }
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(lastUpdate)
                        color: styles.utilityColor
                    }
                }
                RowLayout {
                    Layout.preferredWidth: styles.rowHeight * 3
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: styles.margin
                    RowButton {
                        id: updateButton
                        Layout.alignment: Qt.AlignRight
                        icon.source: "qrc:///qml/icons/refresh.svg"
                        ToolTip.text: qsTr("Update timetable")

                        RotationAnimator {
                            id: rotationAnimation
                            target: updateButton;
                            from: 0;
                            to: 360;
                            duration: 500
                            loops: Animation.Infinite
                            onStopped: {
                                backAnimator.start();
                            }
                        }
                        RotationAnimator {
                            id: backAnimator
                            target: rotationAnimation.target
                            from: updateButton.rotation
                            to: 360
                            duration: ((360-updateButton.rotation)/360)*rotationAnimation.duration
                        }
                        onClicked: {
                            if (styles.animationsEnabled) {
                                rotationAnimation.start();
                            }
                            var newTimetable = localStorage.createTimetable(model.id,
                                                                            model.title,
                                                                            model.isTeacher,
                                                                            api.scheduleSync(model.id,
                                                                                             model.isTeacher))
                            localStorage.save(TableType.SAVED_TIMETABLE,newTimetable);
                            saveModel.replaceItem(newTimetable);

                            if (styles.animationsEnabled) {
                                rotationAnimation.stop();
                            }
                        }
                    }
                    RowButton {
                        id: removeButton
                        Layout.alignment: Qt.AlignRight
                        icon.source: "qrc:///qml/icons/trash.svg"
                        ToolTip.text: qsTr("Remove timetable")
                        onClicked: {
                            localStorage.delete_(TableType.SAVED_TIMETABLE,id)
                            saveModel.removeItem(id)
                        }
                    }
                }
            }
            BottomBorder {
                color: styles.accentColor
            }
        }

    }

}
