import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamaq.alpm.database 1.0

Column {
    property int currentIndex: 0
    id:reposModel
    Repeater{
        anchors.fill: parent
        model:Database.getRepos();
        delegate: ItemDelegate{
            text: modelData
            width:parent.width
            highlighted: index==currentIndex
            onClicked: {
                currentIndex=index;
                mainView.modelData = Database.getRepoPackages(modelData);
            }
        }
    }
}
