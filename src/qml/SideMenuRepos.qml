import QtQuick 2.0
import QtQuick.Controls 2.12
import Pamac.Database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    id:reposModel
    model:Database.getRepos()
    delegate: MenuItemDelegate{
        text: modelData
        onClicked: {
            currentIndex=index;
            mainView.packageListFuture = Database.getRepoPackagesAsync(modelData);
            mainView.title=modelData
        }
    }
    StackView.onActivated: {
        mainView.packageListFuture = Database.getRepoPackagesAsync(model[0]);
        mainView.title=model[0];

    }
}
