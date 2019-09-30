import QtQuick 2.0
import QtQuick.Controls 2.12
import QPamac.Database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: ScrollBar {
        active: true
    }
    model:ListModel{
        ListElement{
            readableName:qsTr("Featured")
            categoryName:"Featured"
        }

        ListElement{
            readableName:qsTr("Utilities")
            categoryName:"Utilities"
        }
        ListElement{
            readableName:qsTr("Music & Audio")
            categoryName:"Music & Audio"
        }
        ListElement{
            readableName:qsTr("Development")
            categoryName:"Development"
        }
        ListElement{
            readableName:qsTr("Education & Science")
            categoryName:"Education & Science"
        }
        ListElement{
            readableName:qsTr("Productivity")
            categoryName:"Productivity"
        }
        ListElement{
            readableName:qsTr("Games")
            categoryName:"Games"
        }
        ListElement{
            readableName:qsTr("Communication & News")
            categoryName:"Communication & News"
        }
        ListElement{
            readableName:qsTr("Photo & Video")
            categoryName:"Photo & Video"
        }
    }
    delegate: MenuItemDelegate{
        text:readableName
        onClicked: {
            currentIndex=index;
            mainView.packageList = Database.getCategoryPackages(categoryName);
            mainView.title = readableName
        }
    }
    StackView.onActivating: {
        mainView.packageList = Database.getCategoryPackages(model.get(currentIndex).categoryName);
        mainView.title = model.get(currentIndex).readableName
    }
}
