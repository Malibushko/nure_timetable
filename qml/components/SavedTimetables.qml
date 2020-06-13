import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import "../styles"
import lib 1.0

Page {
    id: root_
    property alias modelRef: saveModel
    Transition {
        id: removeAnimation
                SequentialAnimation {
                    ColorAnimation {
                        property: "color"
                        to: appSettings.primaryColor
                        duration: 1000
                    }
                    NumberAnimation {
                        property: "y";
                        to: -appSettings.rowHeight
                        duration: 1000
                        easing.type: Easing.OutQuad
                    }
                }
    }

    ListView {
        clip: true
        boundsBehavior: Flickable.OvershootBounds
        anchors.fill: parent
        model: SaveModel {
            id: saveModel
            Component.onCompleted: {
                setItems(localStorage.get(TableType.SAVED_TIMETABLE))
            }
        }
        remove: appSettings.animationsEnabled ? removeAnimation : null

        delegate: Rectangle {
            width: parent.width
            height: appSettings.rowHeight
            color: appSettings.componentColor
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var lessons = model.lessons;
                    console.log(lessons)
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
                    Layout.leftMargin: appSettings.margin
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(title)
                        color: appSettings.textColor
                    }
                    StyledText {
                        Layout.alignment: Qt.AlignLeft
                        text: qsTr(lastUpdate)
                        color: appSettings.utilityColor
                    }
                }
                RowLayout {
                    Layout.preferredWidth: appSettings.rowHeight * 3
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: appSettings.margin
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
                            duration: 10000
                            loops: Animation.Infinite
                        }
                        onClicked: {
                            if (appSettings.animationsEnabled) {
                                rotationAnimation.start();
                            }
                            var newTimetable = localStorage.createTimetable(model.id,
                                                                            model.title,
                                                                            model.isTeacher,
                                                                            api.scheduleSync(model.id,
                                                                                             model.isTeacher))
                            localStorage.save(TableType.SAVED_TIMETABLE,newTimetable);
                            saveModel.replaceItem(newTimetable);

                            if (appSettings.animationsEnabled) {
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
                color: appSettings.accentColor
            }
        }

    }
}
