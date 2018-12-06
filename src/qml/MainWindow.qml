import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0
ApplicationWindow {
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
                toInstall=[];
                toRemove=[];
                toBuild=[];
                toLoad=[];
            }

            transaction.unlock();

        }

    }

    id: mainWindow
    visible: true
    width: 950
    height: 600
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
        state:(toInstall.length>0 ||
               toRemove.length>0 ||
               toBuild.length>0 ||
               toLoad.length>0) ?"opened":"hidden"
    }
    Component.onCompleted: {
        stackView.push()
    }
}


/*##^## Designer {
    D{i:4;anchors_x:553}
}
 ##^##*/
