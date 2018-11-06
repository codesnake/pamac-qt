import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamac.alpm.database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    id:reposModel
    anchors.fill: parent
    model:Database.getRepos()
    delegate: MenuItemDelegate{
        text: modelData
        onClicked: {
            currentIndex=index;
            mainView.modelData = Database.getRepoPackages(modelData);
        }
    }
    Component.onCompleted: {
        mainView.modelData = Database.getRepoPackages(model[0]);
    }
}
