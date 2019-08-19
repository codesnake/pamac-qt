import QtQuick 2.12
import QtQuick.Controls 2.12
Page {
    property string pkgName
    property var opDeps
    property var opted:[]
//    property string title: "Optional dependecies for " + pkgName;
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
        onAccepted: {
            close()
        }
    }
    signal close
}
