import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import Qt.labs.platform 1.0 as Labs
import DialogRunner 1.0
import "./SideMenu" as SideMenu
import "../../js/JSUtils.js" as JSUtils

Page{
    id:sidePanel
    width:Math.min(175,parent.width*0.3)
    property var drawer: drawer
    header:ToolBar{
        padding: 5
        height: toolBar.height
        z:10
        MainContextMenuButton {
            id: toolButton1
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            icon.name: "application-menu"
            width: height
        }
        SearchPane {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: toolButton1.right
            anchors.right: parent.right
            height: parent.height-5
            id: searchPane

        }

    }
    states: [
        State{
            name:"opened"
            PropertyChanges {
                target: sidePanel
                anchors.leftMargin: 0
            }
        },
        State{
            name:"hidden"
            PropertyChanges {
                target: sidePanel
                anchors.leftMargin: -Math.min(175,sidePanel.parent.width*0.3)
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "*"
            animations:[
                NumberAnimation{
                    duration: 200
                    easing.type: Easing.InOutQuad
                    properties: "anchors.leftMargin"
                }
            ]
        }]
    MenuItemDelegate{

        leftPadding: backButtonIcon.x+backButtonIcon.width
        id:backMenuItem
        anchors.top: parent.top
        anchors.topMargin: drawer.depth > 1?0:-height
        anchors.left: parent.left
        width: drawer.width
        text: qsTr("Back")
        Image {
            id:backButtonIcon
            width: height
            height: parent.height/2
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            source: "image://icon/go-previous"
        }
        Behavior on anchors.topMargin {
            SequentialAnimation{

                PauseAnimation {
                    duration: 300
                }
                NumberAnimation{
                    duration: 150
                }
            }
        }

        onClicked: {
            if(drawer.currentItem.objectName=="sideMenuSearch"){
                searchPane.text = "";
            } else{
                drawer.pop();
            }
        }
    }
    SideMenu.Drawer {
        clip: true
        anchors.top: backMenuItem.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        id: drawer
        initialItem: SideMenu.SideMenuMain{
            height: parent.height
        }
    }
    background: Rectangle{
        color: systemPalette.base
        Rectangle{
            color: systemPalette.mid
            width: 1
            anchors.right: parent.right
            height: parent.height
        }
    }
}
