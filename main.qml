import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import lib 1.0
import "./qml/components"

ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    title: qsTr("TimeTable")

    Settings {
        id: appSettings
    }
    header: Header {
        id: mainHeader
    }

    Material.theme: appSettings.appTheme
    Material.accent: appSettings.accentColor
    Material.primary: appSettings.primaryColor
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
            localStorage.save(TableType.SEARCH_GROUP,group)
            findPage.addGroup(group);
        }

        // @disable-check M16
        onTeacherResponse: {
            localStorage.save(TableType.SEARCH_TEACHER,teacher)
            findPage.addTeacher(teacher)
        }
        onTransferingFinished: {
            localStorage.endTransaction();
        }
        onTransferingStarted: {
            localStorage.beginTransaction();
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
        SettingsPage {
            id: settingsPage
        }
        SettingsGroupPage {
            id: settingsGroup
        }
    }
}
