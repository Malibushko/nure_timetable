import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0
import QtQml 2.12
import lib 1.0
import "./qml/components"

ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    title: qsTr("TimeTable")
    function forceRepaint() {
        root.width += 1;
        root.width -=1;
    }

    StyleSettings {
        id: styles
    }

    Settings {
        id: mainSettings
    }
    LanguageSwitcher {
        id: lang
        Component.onCompleted: {
            console.log(mainSettings.value("",SETTINGS_TYPE.CHOSEN_LANGUAGE))
            lang.setLanguage(mainSettings.value("",SETTINGS_TYPE.CHOSEN_LANGUAGE))
        }
    }

    header: Header {
        id: mainHeader
    }

    Material.theme: styles.appTheme
    Material.accent: styles.accentColor
    Material.primary: styles.primaryColor

    MessageDialog {
        id: dialog
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
        // @disable-check M16
        onError: {
            dialog.setData(qsTr("Error"),description)
        }

        Component.onCompleted: {
            if (!findPage.groupInitialized() && styles.cachingEnabled) {
                var cacheResult = localStorage.get(TableType.SEARCH_GROUP);
                if (cacheResult.length > 0) {
                    for (var group in cacheResult) {
                        findPage.addGroup(cacheResult[group])
                    }
                }
                else
                    groups();
            }
            if(!findPage.teachersInitialized() && styles.cachingEnabled) {
                var cacheTeacherResult = localStorage.get(TableType.SEARCH_TEACHER);
                if (cacheTeacherResult.length > 0) {
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
    SettingsPage {
        id: settingsPage
    }
    SettingsGroupPage {
        id: settingsGroup
    }
    FindPage {
        id: findPage
    }
    TimetableTable {
        id: timetablePage
    }
    SavedTimetables {
        id: savedTimetables
    }

    StackView {
        id: mainView
        anchors.fill: parent
        initialItem: savedTimetables
    }
}
