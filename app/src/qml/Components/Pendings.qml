import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import NotificationServices 1.0
import "../../js/JSUtils.js" as Utils
import "../Components/SideMenu" as SideMenu
import "../Components" as Components

Pane{
    padding: 0
    SideMenu.Drawer{
        width: toBuild.length>0?170:0
        id:sideMenuPending
        initialItem:SideMenu.SideMenuPending{
            onCurrentIndexChanged: {
                if(currentIndex==Database.Repos){
                    pendingPackageList.packageList = Database.findPackagesByName(toInstall.concat(toRemove));
                } else{
                    pendingPackageList.packageList = Database.getAurPackages(toBuild);
                }
            }
        }
        anchors{
            left:parent.left
            bottom: parent.bottom
            top:parent.top
        }
        
    }
    
    PackageList{
        id:pendingPackageList
        anchors.left: sideMenuPending.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
}
