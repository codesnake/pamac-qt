import QtQuick 2.0
import QtQuick.Controls 2.2

ListView {
    boundsBehavior: Flickable.StopAtBounds
    model:["Description","Dependecies","Files"]
    delegate:  MenuItemDelegate{
        backgroundColor: systemPalette.alternateBase
        id:packageInfoDelegate
        text:modelData
        onClicked: {
            infoStackLayout.currentIndex = index;
            currentIndex=index;
        }

    }
}