import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12

import lib 1.0
import "./qml/components"


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
            timetablePage.modelRef.addLesson(id,lesson)
        }
        onTimetableAboutToBeArrived: {
            timetablePage.modelRef.prepareForNewTimetable(rowCount)
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
            if (!findPage.groupInitialized()) {
                var cacheResult = localStorage.get(TableType.SEARCH_GROUP);
                if (cacheResult.length > 0) {
                    console.log("Cache hit groups");
                    for (var group in cacheResult) {
                        findPage.addGroup(cacheResult[group])
                    }
                }
                else
                    groups();
            }
            if(!findPage.teachersInitialized()) {
                var cacheTeacherResult = localStorage.get(TableType.SEARCH_TEACHER);
                if (cacheTeacherResult.length > 0) {
                    console.log("Cache hit teachers")
                    for (var teacher in cacheTeacherResult) {
                        findPage.addTeacher(cacheTeacherResult[teacher])
                    }
                }
                else
                    teachers();

            }
        }
    }
    Storage {
        id: localStorage
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
            id: timetablePage
        }
    }
}
