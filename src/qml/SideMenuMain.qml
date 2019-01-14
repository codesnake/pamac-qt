import QtQuick 2.9
import QtQuick.Controls 2.2
import Pamac.Database 1.0

ListView{
    boundsBehavior: Flickable.StopAtBounds

    model:ListModel{
        ListElement{
            name:"Categories"
            url:"SideMenuCategories.qml"
        }
        ListElement{
            name:"Groups"
            url:"SideMenuGroups.qml"
        }
        ListElement{
            name:"Repos"
            url:"SideMenuRepos.qml"
        }
        ListElement{
            name:"Installed"
            url:"SideMenuInstalled.qml"
        }
    }
    delegate:MenuItemDelegate {
        highlighted: false
        text: name
        onClicked:{
            drawer.push(url);
        }
    }
    footer: MenuItemDelegate{
        enabled: toInstall.length>0 || toRemove.length>0
        text: "Pending"
        onClicked:{
            drawer.push("SideMenuPending.qml");
        }
    }
}
