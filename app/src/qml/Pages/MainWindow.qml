import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import Qt.labs.platform 1.0 as Labs
import DialogRunner 1.0
import "../Components" as Components
import "../../js/JSUtils.js" as JSUtils

ApplicationWindow {


    function showUpdates(){
        if(stackView.currentItem.objectName!=="updatesPage")
            stackView.push("UpdatesPage.qml");
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
    property var ignoreWhenUpdate: []

    onClosing: {
        sClosing();
    }

    signal sClosing;

    Transaction{
        id:transaction
        database: Database
        onStartPreparing: {
            if(started){
                details = "";
            }
        }

        Component.onDestruction: {
            transaction.quitDaemon();
        }

        requestChooseProvider: (depend,list)=>{
                                   let objects = {"depend":depend,"lst":list,"provider":undefined}
                                   objects = DialogRunner.exec(Qt.resolvedUrl("../Dialogs/ChooseProviderDialog.qml"),objects)
                                   return objects["provider"]
                               }

        requestOptDepends: (pkgname,lst)=>{
                               let result = DialogRunner.exec(Qt.resolvedUrl("../Dialogs/TransactionOptDependsDialog.qml"),{"pkgName":pkgname,"opDeps":lst,"opted":undefined});
                               return objects["opted"];
                           }

        requestImportKey: (pkgname,key,owner)=>{
                              let objects = {"pkgName":pkgname,"key":key,"owner":owner,"ok":undefined}
                              objects = DialogRunner.exec(Qt.resolvedUrl("../Dialogs/AskImportKeyDialog.qml"),objects);
                              return objects["ok"];
                          }
        requestCommit: summary=>{
                           let objects = {"summary":summary,"result":undefined}
                           objects =  DialogRunner.exec(Qt.resolvedUrl("../Dialogs/TransactionSummaryDialog.qml"),objects);
                           return objects["result"]
                       }
    }

    id: mainWindow
    visible: true
    minimumWidth: 750
    minimumHeight: 450
    width: 950
    height: 550
    objectName: "mainWindow"
    title: qsTr("Pamac Qt - Prerelease")
    SystemPalette{id:systemPalette}

    Item{
        anchors.fill: parent

        focus: true
        Keys.forwardTo: searchPane
        Components.SidePanel {
            id: sidePanel
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            height: parent.height
            state: "opened"

        }

        Page{

            anchors{
                right:parent.right
                top:parent.top
                bottom: parent.bottom
                left:sidePanel.right
            }
            header:Components.ToolBar {
                id: toolBar
                height: 40

            }
            StackView{
                id:stackView
                height: parent.height
                width: parent.width
                clip: true
                initialItem: Components.PackageList {
                    property string title
                    id: mainView

                    onPackageListChanged: {
                        stackView.pop(this);

                    }
                }


            }
        }

    }
    Components.Footer{
        id:bottomPanel
        anchors{
            bottom:parent.bottom
            left:parent.left
            right:parent.right
        }

    }
}
