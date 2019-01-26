import QtQuick 2.11
import QtQuick.Controls 2.12
import Pamac.Database 1.0
ListView{
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
    delegate:MenuItemDelegate {
        text: name
        onClicked:{
            currentIndex=index
            mainView.packageListFuture = Database.getInstalledPackagesAsync(type);
        }
    }
    StackView.onActivated: {
        mainView.packageListFuture = Database.getInstalledPackagesAsync(model.get(0).type);
    }
}
