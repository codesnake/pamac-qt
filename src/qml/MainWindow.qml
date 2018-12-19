import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0

ApplicationWindow {

    function tryLockAndRun(val){
        if(transaction.getLock()){
            val.apply(this, arguments)
        } else{

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

    Transaction{
        id:transaction
        database: Database
        onFinished:{
            if(success){
                clear();
            }
            transaction.unlock();
        }
        onStartedChanged: {
            if(started){
                details = "";
            }
        }
    }

    id: mainWindow
    visible: true
    width: 950
    height: 600
    objectName: "mainWindow"
    title: qsTr("Package manager")

    SystemPalette{id:systemPallette}

    header: PamacToolBar {
        id: toolBar
    }

    StackView{

        id:stackView
        height: parent.height-bottomPanel.height
        width: parent.width
        initialItem: Page{

            SideBar {
                font.bold: true
                id: drawer
                initialItem: SideMenuMain{
                    height: parent.height-updatesItem.height
                    ItemDelegate {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        height: 45

                        id:updatesItem
                        text: qsTr("Updates")
                        width: parent.width

                        onClicked: {
                            stackView.push("UpdatesPage.qml");
                        }
                    }
                }

            }

            SearchPane {
                state: "hidden"
                id: searchPane

            }

            PagePackageTable {
                id: mainView

                modelData: Database.getInstalledApps()

                anchors.right:parent.right
                anchors.top:searchPane.bottom
                anchors.bottom:parent.bottom

                width: mainWindow.width-drawer.width
                clip:true
                focus: true
                Keys.onPressed: {
                    if(event.text!==""){
                        searchPane.state="opened";
                        searchPane.text=event.text;
                    }
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
