import QtQuick 2.0
import QtQuick.Controls 2.2
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
            mainView.modelData = Database.getGroupPackages(modelData);
        }
    }
    Component.onCompleted: {
        mainView.modelData = Database.getGroupPackages(model[0]);
    }
}
