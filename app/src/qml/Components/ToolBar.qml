import QtQuick 2.9
import QtQuick.Controls 2.6
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import NotificationServices 1.0

import "../../js/JSUtils.js" as JSUtils
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
        enabled: stackView.depth > 1 && stackView.currentItem.objectName !== "updatesPage"
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
        font.pixelSize: Qt.application.font.pixelSize*1.2
        anchors.left: toolButton.right
        anchors.verticalCenter: parent.verticalCenter
    }
    Row{
        id:toolButtonsRow
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        Button{
            property var size: 20
            visible:  stackView.currentItem.objectName!="updatesPage"
            id:installedButton
            checkable: true
            checked: drawer.currentItem.objectName == "sideMenuInstalled"
            flat: true
            text: qsTr("Installed")
            icon.name: "dropboxstatus-logo"
            icon.height: size
            icon.width: size
            property var prevSideMenuState
            onClicked: {
                if(checked){
                    prevSideMenuState = drawer.currentItem;
                    drawer.push("SideMenu/SideMenuInstalled.qml");
                }
                else{
                    drawer.pop(prevSideMenuState);
                }
            }
        }

        Button{
            checkable: true
            checked: stackView.currentItem.objectName=="updatesPage"
            id:updatesButton
            flat:true
            icon.name:stackView.currentItem.objectName=="updatesPage"?
                          "go-previous":
                          updatesCount<1?"update-none":"update-medium"
            icon.height: 20
            icon.width: 20
            property int updatesCount:-1
            text: updatesCount<1?qsTr("Updates"):qsTr("Updates (")+updatesCount+")"
            onClicked: {
                if(checked)
                    showUpdates();
                else{
                    stackView.pop();
                }
            }
            Component.onCompleted: {
                let upds = Database.getUpdates();
                let repoUpds = upds.getReposUpdates().length;
                updatesButton.updatesCount = repoUpds;
            }

        }
    }
}
