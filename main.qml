import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import lib 1.0
import "./qml/components"
import Qt.labs.platform 1.1

ApplicationWindow {
    id: root
    visible: true
    width: 1281
    height: 860

    title: qsTr("TimeTable")

    Material.theme: styles.appTheme
    Material.accent: styles.accentColor
    Material.primary: styles.primaryColor


    onClosing: {
        // Close if only if this is the first (main) page
        if (mainView.depth > 1) {
            close.accepted = false;
            mainHeader.backClick()
        } else
            close.accepted = true
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
            lang.setLanguage(mainSettings.value("",SETTINGS_TYPE.CHOSEN_LANGUAGE))
        }
    }

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
    // Pages
    SettingsPage {
        id: settingsPage
    }
    LessonInfo {
        id: lessonInfo
    }
    TableSettingsPage {
        id: tableSettings
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
    header: Header {
        id: mainHeader
    }
    StackView {
        id: mainView
        anchors.fill: parent
        initialItem: savedTimetables
    }
}
