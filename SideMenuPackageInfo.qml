import QtQuick 2.0
import QtQuick.Controls 2.2

ListView {
    boundsBehavior: Flickable.StopAtBounds
    height: parent.height
    width: parent.width
    model:["Description","Dependecies","Files"]
    delegate:  MenuItemDelegate{
        text:modelData
        onClicked: {
            infoStackLayout.currentIndex = index;
            currentIndex=index;
        }

    }
}
