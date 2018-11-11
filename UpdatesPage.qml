import QtQuick 2.0
import QtQuick.Controls 2.4
import Pamac.alpm.database 1.0
Item {
    Connections{
        target: Database
        onUpdatesReady:{
//            updatesPackageTable.modelData = upds.getReposUpdates();
//            updatesPackageTable.visible= true;
        }
        onGetUpdatesProgress:{
            progress.text = "Checking for updates "+percent+"%";
        }
    }
    Label{
        id:progress
        anchors.centerIn: parent
        text:"Checking for updates"
        font.pointSize: 12
        font.bold: true
    }

    PagePackageTable{
        id:updatesPackageTable
        visible: false
        anchors.fill: parent
    }
    Component.onCompleted: {
        Database.getUpdatesAsync()
    }
}

