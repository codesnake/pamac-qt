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
    StackView.onActivating: {
        mainView.packageListFuture = Database.getRepoPackagesAsync(model[currentIndex]);
        mainView.title=model[currentIndex];

    }
}
