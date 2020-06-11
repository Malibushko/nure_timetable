import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12

import "./qml/components"

import lib 1.0
ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    title: qsTr("TimeTable")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    Settings {
        id: appSettings
    }
    header: Header {
        id: mainHeader
    }
    Api {
        id: api
        onNewLesson: {
            timetable.modelRef.addLesson(id,lesson)
        }
        onTimetableAboutToBeArrived: {
            timetable.modelRef.prepareForNewTimetable(rowCount)
        }
        // @disable-check M16
        onGroupResponse: {
            findPage.addGroup(group);
        }
        // @disable-check M16
        onTeacherResponse: {
            findPage.addTeacher(teacher)
        }
        Component.onCompleted: {
            if (!findPage.groupInitialized())
                groups()
            if(!findPage.teachersInitialized())
                teachers()
        }
    }

    StackView {
        id: mainView
        anchors.fill: parent
        initialItem: SavedTimetables {
            id: savedTimetables
        }
        FindPage {
            id: findPage
        }
        TimetableTable {
            id: timetable
        }
    }
}
