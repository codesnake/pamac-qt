import QtQuick 2.0
import QtQuick.Controls 2.12
import Pamac.Database 1.0
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
    StackView.onActivated: {

        if(!Database.config.enableAur){
            drawer.state="hidden";
        }


    }

    Component.onDestruction: {
        drawer.state="opened";
    }
}
