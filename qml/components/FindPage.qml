import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import lib 1.0

Page {
    id: root_

    property int timetableId
    property var timetableTitle
    property bool isTeacher

    function addGroup(group) {
        groupTab.modelRef.sourceModel.addItem(group)
    }
    function addTeacher(teacher) {
        teachersTab.modelRef.sourceModel.addItem(teacher)
    }
    function groupInitialized() {
        return groupTab.modelRef.sourceModel.initialized();
    }
    function teachersInitialized() {
        return teachersTab.modelRef.sourceModel.initialized();
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
            isTeacher: true
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
