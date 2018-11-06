import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamac.alpm.database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    width: parent.width
    model:["Accessories","Audio & Video","Development","Internet","Education","Settings","Science","Games","Office","System Tools","Graphics"]
    delegate: MenuItemDelegate{
        text:modelData
        onClicked: {
            currentIndex=index;
            mainView.modelData = Database.getCategoryPackages(modelData);
        }
    }
    Component.onCompleted: {
        mainView.modelData = Database.getCategoryPackages(model[0]);
    }
}
