import QtQuick 2.9
import QtQuick.Controls 2.2
import Pamaq.alpm.database 1.0

Column {
    property int currentIndex: 0
    width: parent.width

    ItemDelegate {
        highlighted: index==currentIndex
        width: parent.width
        text: "Categories"
        onClicked:{
            currentIndex=0;
            drawer.push("CategoriesSideMenu.qml");
        }
    }
    ItemDelegate {
        highlighted: index==currentIndex
        width: parent.width
        text: "Repos"
        onClicked:{
            currentIndex=1;
            drawer.push("ReposSideMenu.qml");
        }
    }
    ItemDelegate {
        highlighted: index==currentIndex
        width: parent.width
        text: "Installed"
        onClicked:{
            currentIndex=2;
            mainView.modelData = Database.getInstalled();
        }

    }

}
