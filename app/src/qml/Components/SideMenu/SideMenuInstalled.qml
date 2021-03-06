import QtQuick 2.11
import QtQuick.Controls 2.12
import QPamac.Database 1.0
import "../" as Components

ListView{
    objectName: "sideMenuInstalled"
    boundsBehavior: Flickable.StopAtBounds
    model:ListModel{
        ListElement{
            name:qsTr("Installed")
            type: Database.Installed
        }
        ListElement{
            name:qsTr("Explicitly installed")
            type: Database.Explicitly

        }
        ListElement{
            name:qsTr("Orphans")
            type: Database.Orphans

        }
        ListElement{
            name:qsTr("Foreign")
            type: Database.Foreign

        }
    }
    delegate:Components.MenuItemDelegate {
        text: name
        onClicked:{
            currentIndex=index
            mainView.packageList = Database.getInstalledPackages(type);
            mainView.title=name
        }
    }
    StackView.onActivating: {
        mainView.packageList = Database.getInstalledPackages(model.get(currentIndex).type);
        mainView.title=model.get(currentIndex).name
    }
}
