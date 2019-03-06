import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import Pamac.Database 1.0
import Pamac.Package 1.0
Page {
    SideBar{
        state:Database.config.checkAurUpdates?"opened":"hidden"
        id:updatesDrawer
        anchors.left: parent.left
        height: parent.height
        width: 170
        initialItem: ListView{
            id:updatesDrawerListView
            currentIndex: 0
            boundsBehavior: Flickable.StopAtBounds
            model:ListModel{
                ListElement{
                    name:"Repository"
                    type: Database.Repos
                }
                ListElement{
                    name:"AUR"
                    type: Database.AUR

                }

            }

            delegate:MenuItemDelegate {
                highlighted:updatesDrawerListView.currentIndex==index
                text: name
                onClicked:{
                    updatesDrawerListView.currentIndex=index

                }
            }
        }
    }

    title: "Updates"
    objectName: "updatesPage"
    property var updates

    Connections{
        target: Database
        onUpdatesReady:{
            updates = upds;
            var upList = upds.getReposUpdates();
            progress.text = qsTr("System is up to date");

        }
        onGetUpdatesProgress:{
            progress.text = qsTr("Checking for updates ")+percent+"%";
        }
    }
    Column{
        width: implicitWidth
        height: implicitHeight
        spacing: 1
        anchors.centerIn: parent
        BusyIndicator{
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: updates==undefined
            height: enabled?width:0
            width: progress.paintedWidth*0.5
            running: true
            Behavior on height{
                NumberAnimation{}
            }
        }

        Label{


            id:progress
            text:qsTr("Checking for updates")
            font.weight: Font.Bold
            font.pointSize: 12
        }
    }
    PagePackageTable{
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: -1
            radius: 6
            color: systemPalette.dark
        }
        packageList: updatesDrawerListView.currentIndex==Database.Repos?updates.getReposUpdates():updates.getAurUpdates()
        id:updatesPackageTable
        visible: packageList.length>0
        anchors{
            left: updatesDrawer.right
            right: parent.right
            top:parent.top
            bottom: parent.bottom
        }
    }
    Component.onCompleted: {
        drawer.state = "hidden";
        reset();
    }

    StackView.onDeactivating: {
        drawer.state = "opened";
    }

    function reset(){
        updates=undefined
        Database.getUpdatesAsync()
    }

    Connections{
        target: transaction
        onFinished:{
            Database.getUpdatesAsync();
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
