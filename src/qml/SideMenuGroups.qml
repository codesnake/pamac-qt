import QtQuick 2.0
import QtQuick.Controls 2.12
import Pamac.Database 1.0
ListView {
    boundsBehavior: Flickable.StopAtBounds
    id:groupsColumn
    model:Database.getGroups()
    ScrollBar.vertical: ScrollBar {
        active: true
    }
    delegate: MenuItemDelegate{
        text:modelData
        onClicked: {
            currentIndex=index;
            mainView.packageListFuture = Database.getGroupPackagesAsync(modelData);
        }
    }
    StackView.onActivated: {
        mainView.packageListFuture = Database.getGroupPackagesAsync(model[0]);
    }
}
