import QtQuick 2.12
import QtQuick.Controls 2.12
import "../styles"

Dialog {
    id: messageDialog
    modal: true
    standardButtons: Dialog.Ok
    anchors.centerIn: parent
    margins: width * 0.1
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
