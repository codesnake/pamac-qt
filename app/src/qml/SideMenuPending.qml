import QtQuick 2.11
import QtQuick.Controls 2.12
import QPamac.Database 1.0
ListView{
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
        onClicked:{
            currentIndex=index
        }
    }
    ScrollBar.vertical: ScrollBar{
        visible: true
    }
}
