import QtQuick 2.0
import QtQuick.Controls 2.4
import Pamac.Database 1.0
import Pamac.Package 1.0
Item {
    objectName: "updatesPage"
    property var updates
    Connections{
        target: Database
        onUpdatesReady:{
            updates = upds;
            var upList = upds.getReposUpdates();
            updatesPackageTable.modelData = upList;
            updatesPackageTable.visible= true;
            for(var i = 0;i<upList.size;i++)
            {
                toInstall.push(upList.at(i).name);
                toInstallChanged();
            }

        }
        onGetUpdatesProgress:{
            progress.text = "Checking for updates "+percent+"%";
        }
    }
    Label{
        id:progress
        anchors.centerIn: parent
        text:"Checking for updates"
        font.weight: Font.Bold
        font.pointSize: 12
        font.bold: false

    }

    PagePackageTable{
        id:updatesPackageTable
        visible: false
        anchors.fill: parent
    }
    Component.onCompleted: {
        Database.getUpdatesAsync()
    }
    Component.onDestruction: {
        toInstall=[];
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
