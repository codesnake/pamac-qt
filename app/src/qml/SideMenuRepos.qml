import QtQuick 2.0
import QtQuick.Controls 2.12
import QPamac.Database 1.0

ListView {
    boundsBehavior: Flickable.StopAtBounds
    id:reposModel
    model:Database.getRepos()
    delegate: MenuItemDelegate{
        text: modelData
        onClicked: {
            currentIndex=index;
            mainView.packageList = Database.getRepoPackages(modelData);
            mainView.title=modelData
        }
    }
    StackView.onActivating: {
        mainView.packageList = Database.getRepoPackages(model[currentIndex]);
        mainView.title=model[currentIndex];

    }
}
