import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0
import "./" as PamacQt
import "../js/JSUtils.js" as JSUtils

ApplicationWindow {

    function tryLockAndRun(val){
        if(transaction.getLock()){
            val.apply(arguments);
        } else{
            retryTimer.func = val;
            retryTimer.args = arguments;
            retryDialog.open();
            retryTimer.start();
        }
    }



    Dialog{

        title: ""
        id:retryDialog
        Timer{
            repeat: true
            property var func
            property var args
            id:retryTimer
            running: false
            onTriggered: {
                if(transaction.getLock()){
                    retryTimer.stop();
                    retryDialog.close();
                    func.apply(mainWindow,args);
                }
            }
        }
        onVisibleChanged: {
            if(!visible){
                retryTimer.stop();
            }
        }
        standardButtons: Dialog.NoButton
        Label{
            text: "Waiting for another package manager to quit"
        }
    }

    function clear(){
        toInstall=[];
        toRemove=[];
        toBuild=[];
        toLoad=[];
    }

    property var toInstall: []
    property var toRemove: []
    property var toBuild: []
    property var toLoad: []

    AboutDialog{
        id:aboutDialog
    }
    PreferencesDialog{
        id:preferencesDialog
        onRejected: {
            transaction.unlock();
        }
    }

    Dialog{
        height: 400
        width: 300
        id:historyDialog
        ScrollView{
            anchors.fill: parent
            TextArea{
                readOnly: true
            }
        }
    }

    Transaction{
        id:transaction
        database: Database
        onStartPreparing: {
            if(started){
                details = "";
            }
        }

        onFinished:{
            if(success){
                clear();
            }
            transaction.unlock();
        }
    }

    id: mainWindow
    visible: true
    minimumWidth: 750
    minimumHeight: 450
    width: 950
    height: 550
    objectName: "mainWindow"
    title: qsTr("Package manager")
    SystemPalette{id:systemPalette}

    header: PamacQt.ToolBar {
        id: toolBar
    }

    StackView{

        focus: true
        Keys.enabled: true
        Keys.onPressed: {
            if(event.text!==""){
                searchPane.state="opened";
                searchPane.forceActiveFocus();
                searchPane.append(event.text);
            }
        }

        id:stackView
        height: parent.height-bottomPanel.height
        width: parent.width
        initialItem: Page{
            SideBar {
                height: stackView.height
                font.bold: true
                id: drawer
                initialItem: SideMenuMain{
                    height: parent.height-updatesItem.height
                    MenuItemDelegate {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        height: 45
                        leftPadding: 15
                        id:updatesItem
                        text: qsTr("Updates")
                        width: parent.width

                        onClicked: {
                            stackView.push("UpdatesPage.qml");
                        }
                    }
                }

            }
            Item {
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: -1
                    radius: 6
                    color: systemPalette.dark
                }

                anchors{
                    right:parent.right
                    top:parent.top
                    bottom:parent.bottom
                }
                width: mainWindow.width-drawer.width

                SearchPane {
                    anchors{
                        top:parent.top
                        right:parent.right
                        left:parent.left
                    }
                    state: "hidden"
                    id: searchPane

                }
                PagePackageTable {

                    id: mainView


                    anchors{
                        left: parent.left
                        right: parent.right
                        bottom:parent.bottom
                        top:searchPane.bottom
                    }

                    clip:true

                }
            }
        }
    }
    BottomPanel{
        id:bottomPanel
        anchors{
            bottom:parent.bottom
            left:parent.left
            right:parent.right
        }
        state:(transaction.started ||
               toInstall.length>0 ||
               toRemove.length>0 ||
               toBuild.length>0 ||
               toLoad.length>0 ||
               sysUpgrade) ?"opened":"hidden"
    }


}



/*##^## Designer {
    D{i:4;anchors_x:553}
}
 ##^##*/
