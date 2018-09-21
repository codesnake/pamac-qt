import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamaq.alpm.database 1.0

Column {
    property int currentIndex: 0
    width: parent.width
    Repeater{
        width: parent.width
        model:["Graphics","Games","Internet","Multimedia","Settings","Science","Other"]
        delegate: ItemDelegate{
            highlighted: index==currentIndex
            width: parent.width
            text:modelData
            onClicked: {
                currentIndex=index;
                mainView.modelData = Database.getCategoryPackages(modelData);
            }
        }
    }
}
