import QtQuick 2.0
import QtQuick.Controls 2.2

Column {
    property int currentIndex: 0
    Repeater{
        width: parent.width
        model:["Description","Dependecies","Files"]
        delegate:  ItemDelegate{
            width: parent.width
            highlighted: currentIndex==index
            text:modelData
            onClicked: {
                infoStackLayout.currentIndex = index;
                currentIndex=index;
            }

        }
    }
}
