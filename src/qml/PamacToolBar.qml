import QtQuick 2.9
import QtQuick.Controls 2.6
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import QtQuick.Dialogs 1.2
import "../js/JSUtils.js" as JSUtils
ToolBar {

    id: toolBar
    padding: 5
    contentHeight: toolButton.implicitHeight
    width:parent.width
    ToolButton {
        anchors.verticalCenter: parent.verticalCenter
        flat:true
        id: toolButton
        width: height
        icon.name: "go-previous"
        font.pixelSize: Qt.application.font.pixelSize * 1.4
        enabled: stackView.depth > 1 || drawer.depth > 1
        visible: enabled
        onClicked: {
            if (stackView.depth > 1) {
                stackView.pop()
            }
            else if(drawer.depth>1){
                drawer.pop();
                mainView.modelData=Database.getInstalledApps()
            }
        }
    }
    
    
    Label {
        text: stackView.currentItem.title!==undefined?stackView.currentItem.title:""
        anchors.centerIn: parent
    }

    ToolButton {
        id: toolButton2
        icon.name: "search"
        anchors.right: toolButton1.left
        anchors.rightMargin: 0
        checkable: true
        checked: searchPane.state=="opened"
        onClicked: {
            if(searchPane.state!="opened"){
                searchPane.state="opened";
                searchPane.forceActiveFocus();
            }
            else{
                searchPane.text = "";
                searchPane.state="hidden";
            }
        }
    }

    ToolButton {
        checkable: true;
        id: toolButton1
        anchors.right: parent.right
        width: height
        anchors.verticalCenterOffset: 0
        flat: true
        icon.name: "application-menu"
        font.pixelSize: Qt.application.font.pixelSize * 1.4
        anchors.verticalCenter: parent.verticalCenter

        FileDialog{
            id:fileDialog
            title: qsTr("Install Local Packages")
            folder: shortcuts.home
            nameFilters: ["Pacman packages (*.pkg.tar.xz)"]
            onAccepted: {
                for(var i =0;i<fileUrls.length;i++){
                    toLoad.push(fileUrls[i].toString());
                }

                if(transaction.getLock())
                    transaction.start([],[],toLoad,[],[],[]);
            }
        }
        Menu {
            visible: toolButton1.checked
            y: toolButton1.height
            id: contextMenu
            Action {
                text: "Refresh databases"
                onTriggered: {
                    if(transaction.getLock()){
                        transaction.startSysupgrade(true,false,[],[]);
                    }
                }
            }
            Action { text: "View History" }
            Action {


                text: "Install local packages"
                onTriggered: {
                    fileDialog.visible = true;
                }
            }
            Action {
                text: "Preferences"
                onTriggered: {
                    if(transaction.getLock()){
                        JSUtils.connectOnce(transaction.getAuthorizationFinished,function(bool){
                            if(bool)
                                preferencesDialog.open();
                        });
                        transaction.startGetAuthorization();
                    }
                }
            }
            Action {
                text: "About"
                onTriggered: {
                    aboutDialog.open();
                }
            }
        }
    }

}
