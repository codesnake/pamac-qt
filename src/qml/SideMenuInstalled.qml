import QtQuick 2.0
import Pamac.Database 1.0
ListView{
    boundsBehavior: Flickable.StopAtBounds
    width: parent.width
    model:ListModel{
        ListElement{
            name:"Installed"
            type: Database.Installed
        }
        ListElement{
            name:"Explicitly installed"
            type: Database.Explicitly

        }
        ListElement{
            name:"Orphans"
            type: Database.Orphans

        }
        ListElement{
            name:"Foreign"
            type: Database.Foreign

        }
    }
    delegate:MenuItemDelegate {
        text: name
        onClicked:{
            currentIndex=index
            mainView.modelData = Database.getInstalledPackages(type);
        }
    }
    Component.onCompleted: {
        mainView.modelData = Database.getInstalledPackages(Database.Installed);
    }
}
