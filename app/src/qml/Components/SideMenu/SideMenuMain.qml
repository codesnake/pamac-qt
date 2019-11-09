import QtQuick 2.9
import QtQuick.Controls 2.12
import QPamac.Database 1.0
import "../" as Components
ListView{
    boundsBehavior: Flickable.StopAtBounds

    model:ListModel{
        ListElement{
            name:qsTr("Categories")
            url:"SideMenuCategories.qml"
        }
        ListElement{
            name:qsTr("Groups")
            url:"SideMenuGroups.qml"
        }
        ListElement{
            name:qsTr("Repos")
            url:"SideMenuRepos.qml"
        }
    }
    delegate:Components.MenuItemDelegate {
        highlighted: false
        text: name
        onClicked:{
            drawer.push(url);
        }
    }
    StackView.onActivating: {
        mainView.packageList = Database.getInstalledApps();

        mainView.title=qsTr('Installed Apps');
    }
    Component.onCompleted: {
        mainView.packageList = Database.getInstalledApps();

        mainView.title=qsTr('Installed Apps');
    }
    ScrollBar.vertical: ScrollBar{
        visible: true
    }
}
