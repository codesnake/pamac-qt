import QtQuick 2.0
import QtQuick.Controls 2.12
import QPamac.Database 1.0
import "../" as Components

ListView {
    boundsBehavior: Flickable.StopAtBounds
    id:groupsColumn
    model:Database.getGroups()
    ScrollBar.vertical: ScrollBar {
        active: true
    }
    delegate: Components.MenuItemDelegate{
        text:modelData
        onClicked: {
            currentIndex=index;
            mainView.packageList = Database.getGroupPackages(modelData);
            mainView.title=modelData
        }
    }
    StackView.onActivating: {
        mainView.packageList = Database.getGroupPackages(model[currentIndex]);
        mainView.title=model[currentIndex]
    }
}
