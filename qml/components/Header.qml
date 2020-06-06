import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../styles"
ToolBar {
    height: appSettings.rowHeight
    Row {
        anchors.fill: parent
        BurgerButton {
            id: sideMenuButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: appSettings.margin
            scale: parent.height/(Math.max(height*2,1))
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sideMenuButton.state = sideMenuButton.state === "menu" ? "back" : "menu"
                }
            }
        }
        BurgerButton {
            id: backButton
            state: "back"
            visible: mainView.depth > 1
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: appSettings.margin
            scale: parent.height/(Math.max(height*2,1))
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // back
                    // rotate animation

                }
            }
        }
    }
}
