import QtQuick 2.0
import QtQuick.Controls 2.12
import QPamac.Database 1.0
ListView{
objectName: "sideMenuSearch"
    boundsBehavior: Flickable.StopAtBounds
    model:ListModel{
        ListElement{
            name:"Repository"
            type: Database.Repos
        }
        ListElement{
            name:"AUR"
            type: Database.AUR

        }

    }

    delegate:MenuItemDelegate {
        text: name

        height:(!Database.config.enableAur && index==Database.AUR)?-spacing:35
        onClicked:{
            currentIndex=index

        }
    }
}
