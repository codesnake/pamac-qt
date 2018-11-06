import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.alpm.database 1.0
import Pamac.alpm.packageModel 1.0
ApplicationWindow {
    AboutDialog{
        id:aboutDialog
    }

    id: window
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
                        Connections{
                            target: Database
                            onUpdatesReady:{
                                stackView.push("PagePackageTable.qml",{modelData:upds.getReposUpdates()});
                            }
                        }

                        onClicked: {
                            Database.getUpdatesAsync()
                        }
                    }
                }

            }


            PagePackageTable {
                id: mainView

                modelData: Database.getInstalledApps()

                anchors.right:parent.right
                anchors.top:parent.top
                anchors.bottom:parent.bottom

                width: window.width-drawer.width
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
    }
}


/*##^## Designer {
    D{i:4;anchors_x:553}
}
 ##^##*/
