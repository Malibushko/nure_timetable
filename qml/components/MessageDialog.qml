import QtQuick 2.12
import QtQuick.Controls 2.12
import "../styles"

Dialog {
    id: messageDialog
    modal: true
    standardButtons: Dialog.Ok
    anchors.centerIn: parent
    width: root.width * 0.9
    height: (messageText.lineCount+1) * styles.rowHeight
    StyledText {
        id: messageText
        anchors.fill: parent
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }
    function setData(messageTitle,messageDescription) {
        title = messageTitle;
        messageText.text = messageDescription
        messageDialog.open()
    }
}
