import QtQuick 2.0
import QtQuick.Controls 2.12
import "../" as Components

ListView {
    boundsBehavior: Flickable.StopAtBounds
    model:["Description","Dependencies","Files"]
    delegate:  Components.MenuItemDelegate{
//        backgroundColor: systemPalette.alternateBase
        id:packageInfoDelegate
        text:modelData
        onClicked: {
            infoStackLayout.currentIndex = index;
            currentIndex=index;
        }

    }
}
