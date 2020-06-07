import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import lib 1.0

Page {
    id: root_
    function addGroup(group) {
        groupTab.modelRef.addItem(group)
    }
    function addTeacher(teacher) {
        teachersTab.modelRef.addItem(teacher)
    }

    Api {
        id: api
        onGroupResponse: {
            addGroup(group)
        }
        onTeacherResponse: {
            addTeacher(teacher)
        }
        Component.onCompleted: {
            if (!groupTab.modelRef.initialized())
                groups()
            if (!teachersTab.modelRef.initialized())
                teachers()

        }
    }

    StackLayout {
        anchors.fill: parent
        width: parent.width
        currentIndex: bar.currentIndex
        SearchComponent {
            id: groupTab
            Component.onCompleted: {
                groupTab.textInputRef.placeholderText = qsTr("ITKH-18-9")
            }
        }
        SearchComponent {
            id: teachersTab
            Component.onCompleted: {
                teachersTab.textInputRef.placeholderText = qsTr("Степан Андреевич Бандера")
            }
        }
    }

    footer: TabBar {
        id: bar
        TabButton {
            text: qsTr("Students")
        }
        TabButton {
            text: qsTr("Teachers")
        }
    }
}
