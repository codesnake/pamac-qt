import QtQuick 2.0
import QtQuick.Controls 2.12

ListView {
    boundsBehavior: Flickable.StopAtBounds
    model:["Description","Dependencies","Files"]
    delegate:  MenuItemDelegate{
//        backgroundColor: systemPalette.alternateBase
        id:packageInfoDelegate
        text:modelData
        onClicked: {
            infoStackLayout.currentIndex = index;
            currentIndex=index;
        }

    }
}
