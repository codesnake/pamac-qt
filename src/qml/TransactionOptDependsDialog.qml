import QtQuick 2.9
import QtQuick.Controls 2.4
Item {

    property string pkgName
    property var opDeps
    property var opted:[]
    width: 600
    height: 400
    Pane{
        anchors{
            margins: 6
            top:parent.top
            bottom: buttonBox.bottom
            left: parent.left
            right: parent.right
        }
        ListView{
            anchors.fill: parent

            model: opDeps
            delegate: CheckBox{
                onCheckedChanged: {
                    opted[index]=checked
                }

                width: parent.width
                text: modelData
            }
        }
    }
    DialogButtonBox{

        id:buttonBox
        anchors{
            margins: 6
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        standardButtons: DialogButtonBox.Ok
        onAccepted: parent.accepted()
        onRejected: parent.rejected()
    }
    signal accepted
    signal rejected

    function onShow(){
        for(var i = 0;i<opDeps.length;i++){
            opted[i]=false;
        }
    }
}
