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
import "./" as PamacQt
import "../js/JSUtils.js" as JSUtils

Page{
    id:sidePanel
    width:Math.min(175,parent.width*0.3)
    header:ToolBar{
        padding: 5
        height: toolBar.height
        z:10
        ToolButton {
            checkable: true;
            id: toolButton1
            anchors.left: parent.left
            width: height
            flat: true
            icon.name: "application-menu"
            font.pixelSize: Qt.application.font.pixelSize * 1.4
            anchors.verticalCenter: parent.verticalCenter
            
            
            Menu {
                onClosed: toolButton1.checked=false
                
                
                visible: toolButton1.checked
                y: toolButton1.height
                id: contextMenu
                Action {
                    text: "Refresh databases"
                    onTriggered: {
                        transaction.startSysupgrade(true,false,[],[]);
                        
                    }
                }
                Action { text: "View History"
                    onTriggered: {
                        DialogRunner.exec("qrc:/src/qml/HistoryDialog.qml");
                    }
                }
                Labs.FileDialog{
                    id: fileDialog
                    fileMode: Labs.FileDialog.OpenFiles
                    nameFilters: ["ALPM packages (*.pkg.tar.*)"]
                    onAccepted: {
                        transaction.start([],[],fileDialog.files.map(value=>value.toString().replace("file://","")))
                    }
                }
                
                Action {
                    text: "Install local packages"
                    onTriggered: {
                        fileDialog.open()
                    }
                }
                Action {
                    text: "Preferences"
                    onTriggered: {
                        DialogRunner.exec(Qt.resolvedUrl("PreferencesDialog.qml"),{"transaction":transaction})
                    }
                }
                Action {
                    text: "About"
                    onTriggered: {
                        DialogRunner.exec(Qt.resolvedUrl("AboutDialog.qml"));
                    }
                }
            }
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
    SideBar {
        clip: true
        anchors.top: backMenuItem.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        id: drawer
        initialItem: SideMenuMain{
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
