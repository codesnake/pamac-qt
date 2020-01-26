import QtQuick 2.0
import QtQuick.Controls 2.12
import "../" as Components

ListView {
    boundsBehavior: Flickable.StopAtBounds
    model: ["Description","Dependecies","Build files"]
    delegate: Components.MenuItemDelegate{
        id:packageInfoDelegate
        text:modelData
        onClicked: {
            infoStackLayout.currentIndex = index;
            currentIndex=index;
        }

    }
}
