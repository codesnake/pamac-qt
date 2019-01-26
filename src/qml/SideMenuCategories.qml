import QtQuick 2.0
import QtQuick.Controls 2.12
import Pamac.Database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: ScrollBar {
        active: true
    }
    model:ListModel{
        ListElement{
            readableName:qsTr("Accessories")
            categoryName:"Utility"
        }
        ListElement{
            readableName:qsTr("Audio & Video")
            categoryName:"AudioVideo"
        }
        ListElement{
            readableName:qsTr("Development")
            categoryName:"Development"
        }
        ListElement{
            readableName:qsTr("Internet")
            categoryName:"Network"
        }
        ListElement{
            readableName:qsTr("Education")
            categoryName:"Education"
        }
        ListElement{
            readableName:qsTr("Settings")
            categoryName:"Settings"
        }
        ListElement{
            readableName:qsTr("Science")
            categoryName:"Science"
        }
        ListElement{
            readableName:qsTr("Games")
            categoryName:"Game"
        }
        ListElement{
            readableName:qsTr("Office")
            categoryName:"Office"
        }
        ListElement{
            readableName:qsTr("System Tools")
            categoryName:"System"
        }
        ListElement{
            readableName:qsTr("Graphics")
            categoryName:"Graphics"
        }

    }
    delegate: MenuItemDelegate{
        text:readableName
        onClicked: {
            currentIndex=index;
            mainView.packageListFuture = Database.getCategoryPackagesAsync(categoryName);
        }
    }
    StackView.onActivated: {
        mainView.packageListFuture = Database.getCategoryPackagesAsync(model.get(0).categoryName);
    }
}
