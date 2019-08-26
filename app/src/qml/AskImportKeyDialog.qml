import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    property string pkgName
    property string key
    property string owner
    property bool ok

    anchors.fill: parent

    Label{
        anchors.centerIn: parent
        text: "The following key should be imported for\n"+pkgName+"\n"+key+"\nOwner: "+owner;
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    DialogButtonBox{
        id:buttonBox
        anchors{
            margins: 6
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
        onAccepted: dialogAccepted()
        onRejected: dialogRejected()
    }
    signal dialogAccepted
    onDialogAccepted: {
        ok = true;
        close();
    }

    signal dialogRejected
    onDialogRejected: {
        ok = false;
        close();
    }
    signal close
}
