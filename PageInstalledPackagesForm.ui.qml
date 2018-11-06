import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.alpm.packageModel 1.0
import Pamac.alpm.database 1.0
Page {
    font.pixelSize: 8
    title: qsTr("Installed")


    TableView{
        model:AlpmPackageModel{
            id:localModel;
            // @disable-check M222
            packageList: Database.getInstalled();
        }
    }
}
