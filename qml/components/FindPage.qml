import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Page {
    id: root_

    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex
        Item {
            id: teachersTab
            Text {
                text:"Teachers"
            }
        }
        Item {
            id: studentsTab
            Text {
                text: "Students"
            }
        }
        Item {
            id: searchTab
            Text {
                text: "Students"
            }
        }

    }

    header: TabBar {
        id: bar
        TabButton {
            text: qsTr("Students")
        }
        TabButton {
            text: qsTr("Teachers")
        }
        TabButton {
            text: qsTr("Search")
        }
    }
}
