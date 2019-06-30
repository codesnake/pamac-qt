import QtQuick 2.9
import QtQuick.Controls 2.6
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QtQuick.Dialogs 1.2
import "../js/JSUtils.js" as JSUtils
ToolBar {
    height: 40
    id: toolBar
    padding: 5
    contentHeight: toolButton.implicitHeight
    width:parent.width
    clip: true
    ToolButton {
        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: toolButton
                    anchors.leftMargin: 0
                }
                PropertyChanges {
                    target: toolButton
                    opacity:1.0
                }
            },
            State {
                name: "hidden"
                PropertyChanges {
                    target: toolButton
                    anchors.leftMargin: -20
                }
                PropertyChanges {
                    target: toolButton
                    opacity:0.0
                }
            }
        ]
        transitions: [
            Transition {
                PropertyAnimation{
                    property:"anchors.leftMargin"
                }

                PropertyAnimation{
                    property: "opacity"
                }

            }
        ]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        flat:true
        id: toolButton
        width: height
        icon.name: "go-previous"
        font.pixelSize: Qt.application.font.pixelSize * 1.4
        enabled: stackView.depth > 1
        visible: enabled
        state:visible?"visible":"hidden"
        onClicked: {
            if (stackView.depth > 1) {
                stackView.pop();
            }
        }
    }



    Label {
        text: stackView.currentItem.title!==undefined?stackView.currentItem.title:""
        font.pointSize: 12
        anchors.left: toolButton.right
        anchors.verticalCenter: parent.verticalCenter
    }
    Button{
        checkable: true
        checked: stackView.currentItem.objectName=="updatesPage"
        id:updatesButton
        flat:true
        icon.name: "update-none"
        icon.height: 20
        icon.width:icon.height
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        property int updatesCount:-1
        text: updatesCount<1?qsTr("Updates"):qsTr("Updates (")+updatesCount+")"
        onClicked: {

            if(stackView.currentItem.objectName=="updatesPage"){
                stackView.pop();
            } else{
                showUpdates();
            }
        }
        Component.onCompleted: {
            Database.getUpdatesAsync();
        }
        Connections{
            target: Database
            onUpdatesReady: {
                updatesButton.updatesCount = upds.getReposUpdates().size
            }
        }
    }

}
