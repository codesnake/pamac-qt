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
            itemVisible:true
        }
        ListElement{
            name:"AUR"
            type: Database.AUR

        }
        Component.onCompleted: {
            this.get(1).itemVisible = Database.config.enableAur
        }
    }

    delegate:MenuItemDelegate {
        text: name

        height:35
        
        visible:  itemVisible
        onClicked:{
            currentIndex=index

        }
    }
}
