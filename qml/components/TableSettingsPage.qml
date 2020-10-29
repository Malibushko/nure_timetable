import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4 as ControlStyles

import "../styles"
import lib 1.0
Dialog {
   id: root_
   property alias modelRef: settingsModel

   anchors.centerIn: overlay
   width: styles.rowWidth*0.75
   height: root.height * 0.6
   modal: true
   closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape | Popup.CloseOnPressOutside
   onClosed:  {
       timetablePage.modelRef.redrawHideLessons()
       localStorage.save(TableType.SAVED_TIMETABLE,
        savedTimetables.modelRef.createTimetable(timetablePage.modelRef.id(),
                                                 timetablePage.modelRef.getHideLessonList()));
   }

   header: Rectangle {
       height: parent.height * 0.1
       StyledText {
           anchors.verticalCenter: parent.verticalCenter
           anchors.left: parent.left
           anchors.leftMargin: styles.margin
           color: styles.iconColor
           text: qsTr("Timetable settings")
           font.bold: true
       }
       color: styles.themeColor
   }
   TreeModel {
           id: treemodel
           roles: ["name"]
           TreeNode {
               TreeNode {
                   property string name: qsTr("Тип");
               }
               TreeNode {
                   property string name: qsTr("Время");
               }
           }
   }
   contentItem: Controls.TreeView {
       id: settingsTree
       model: TreeModel {
           id: settingsModel
       }
       verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
       anchors.top: root_.header.bottom
       anchors.right: parent.right
       anchors.bottom: parent.bottom
       anchors.left: parent.left
       backgroundVisible: false
       Controls.TableViewColumn {
            role: "name"
       }
       itemDelegate: Rectangle {
           color: hoverCatch.containsMouse ? Qt.darker(styles.componentColor,1.02) : styles.componentColor
           StyledText {
               anchors.left: parent.left
               anchors.verticalCenter: parent.verticalCenter
               anchors.margins: styles.margin + parent.width * 0.03 * styleData.depth
               text: model.subject
           }
           Switch {
               id: settingsSwitch
               anchors.right: parent.right
               anchors.verticalCenter: parent.verticalCenter
               anchors.margins: styles.margin
               checked: model.checked
               visible: !styleData.hasChildren
           }
           Image {
               source: "qrc:///qml/icons/down_triangle.svg"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: styles.margin
                height: styles.iconSize
                width: styles.iconSize
                visible: !styleData.depth
                rotation: styleData.isExpanded ? 180 : 0
           }

           MouseArea {
               id: hoverCatch
               anchors.fill: parent
               propagateComposedEvents: true
               hoverEnabled: styles.animationsEnabled
               onHoveredChanged: {
                   parent.border.color = containsMouse ? styles.accentColor
                                                       : "transparent"
                   parent.border.width = containsMouse
               }
               onClicked: {
                   if (styleData.hasChildren) {
                       if (!settingsTree.isExpanded(styleData.index))
                           settingsTree.expand(styleData.index)
                       else
                           settingsTree.collapse(styleData.index)
                   } else {
                       settingsSwitch.checked = !settingsSwitch.checked
                       settingsModel.changeVisible(styleData.index,settingsSwitch.checked)
                   }
               }
           }
           BottomBorder{}
       }

       style: ControlStyles.TreeViewStyle {
             indentation:  0;
             headerDelegate: Rectangle{
                 color: "transparent"
                 width: 0
                 height: 0
             }
             rowDelegate: Rectangle {
                            height:  styles.rowHeight * 0.75
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: styles.componentColor
             }
        backgroundColor: "transparent"
       }
       alternatingRowColors: false
   }
}
