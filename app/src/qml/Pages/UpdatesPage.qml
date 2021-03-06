import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.Package 1.0
import "../../js/JSUtils.js" as JSUtils
import "../Components" as Components
import "../Components/SideMenu" as SideMenu
Page {


    objectName: "updatesPage"
    property var updates

    Connections{
        target: Database
        onGetUpdatesProgress:{
            progressBar.value = percent
        }
    }
    Column{
        width: implicitWidth
        height: implicitHeight
        spacing: progressBar.enabled?10:-5
        anchors.centerIn: parent
        Components.ProgressBar{
            id:progressBar
            enabled: updates===undefined
            height: enabled?5:0
            width: progress.paintedWidth*2
            from:0
            to:100
            Behavior on height{
                NumberAnimation{}
            }
        }

        Label{
            anchors.horizontalCenter: parent.horizontalCenter
            id:progress
            text:qsTr("Checking for updates")
            font.weight: Font.Bold
            font.pointSize: 12
        }
        Behavior on spacing{
            NumberAnimation{}
        }
    }
    Item{
        opacity: JSUtils.isAccessible(updates)
        Behavior on opacity {
            NumberAnimation{duration: 300}
        }

        anchors.fill: parent
        SideMenu.Drawer{
            visible: {
                if (updates===undefined){
                    return false
                }

                let result = updates.getReposUpdates().length>0

                if(Database.config.enableAur){
                    result |= updates.getAurUpdates().length>0
                }
                return result;
            }
            states: [
                State{
                    name:"opened"
                    PropertyChanges {
                        target: updatesDrawer
                        width:Math.min(175,parent.width*0.3)
                    }
                },
                State{
                    name:"hidden"
                    PropertyChanges {
                        target: updatesDrawer
                        width:0
                    }
                }
            ]
            state:updatesDrawer.visible?"opened":"hidden"
            id:updatesDrawer
            anchors.left: parent.left
            height: parent.height
            initialItem: ListView{
                id:updatesDrawerListView
                currentIndex: 0
                boundsBehavior: Flickable.StopAtBounds
                model:ListModel{
                    id:drawerListViewModel
                    ListElement{
                        name:"Repository"
                        type: Database.Repos
                        enable:false
                    }
                    ListElement{
                        name:"AUR"
                        type: Database.AUR
                        enable:false
                    }

                }

                delegate:Components.MenuItemDelegate {
                    highlighted:updatesDrawerListView.currentIndex==index
                    text: name
                    enabled: enable
                    onClicked:{
                        updatesDrawerListView.currentIndex=index

                    }
                }
            }
        }
        Components.PackageList{
            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: -1
                radius: 6
                color: systemPalette.dark
            }
            packageList: JSUtils.isAccessible(updates)?
                             updatesDrawerListView.currentIndex==Database.Repos?
                                 updates.getReposUpdates():
                                 updates.getAurUpdates():
                                 []
            id:updatesPackageTable
            visible: !JSUtils.isAccessible(updates) || (JSUtils.isAccessible(packageList) && packageList.length>0)
            anchors{
                left: updatesDrawer.right
                right: parent.right
                top:parent.top
                bottom: parent.bottom
            }
        }
    }
    StackView.onActivating: {
        sidePanel.state = "hidden";
    }
    StackView.onActivated: {
        reset();
    }
    StackView.onDeactivating: {
        sidePanel.state = "opened";
    }

    function reset(){
        progress.text = qsTr("Checking for updates...");
        updates = Database.getUpdates();
        let hasRepo = updates.getReposUpdates().length>0;
        let hasAur = updates.getAurUpdates().length>0;
        drawerListViewModel.setProperty(0,"enable",hasRepo);
        drawerListViewModel.setProperty(1,"enable",hasAur);
        if(!hasRepo)
            updatesDrawerListView.currentIndex=1
        if(!hasRepo && !hasAur){
            progress.text = qsTr("System is up to date");
        } else{
            progress.text = qsTr("Showing updates")
        }
    }

    Connections{
        target: transaction
        onFinished:{
            reset()
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
