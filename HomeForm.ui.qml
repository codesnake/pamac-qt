import QtQuick 2.9
import QtQuick.Controls 2.2
import Pamaq.alpm.packageModel 1.0
import Pamaq.alpm.database 1.0

Page {

    title: qsTr("Home")

    TableView{
        model:AlpmPackageModel{
            id:localModel;
            // @disable-check M222
            packageList: Database.getInstalledApps();
        }
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
