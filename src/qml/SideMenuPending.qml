import QtQuick 2.11
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
        height:(!Database.config.enableAur && index==Database.AUR)?-spacing:35
        onClicked:{
            currentIndex=index
            if(index==Database.Repos){
                mainView.packageList= Database.getPending(toInstall,toRemove);
            } else{
                mainView.packageList = undefined
            }
        }
    }
    StackView.onActivated: {
        if(currentIndex==Database.Repos){
            mainView.packageList= Database.getPending(toInstall,toRemove);
        } else{
            mainView.packageList = undefined
        }
    }
}
