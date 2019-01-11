import QtQuick 2.0
import QtQuick.Controls 2.4
import Pamac.Database 1.0
import Pamac.Package 1.0
Page {
    title: "Updates"
    objectName: "updatesPage"
    property var updates
    Connections{
        target: Database
        onUpdatesReady:{
            updates = upds;
            var upList = upds.getReposUpdates();
            if(upList.length>0){
                updatesPackageTable.modelData = upList;
                updatesPackageTable.visible= true;
            } else {
                updatesPackageTable.modelData = undefined;
                updates=undefined;
                progress.text = qsTr("No updates available");
            }

        }
        onGetUpdatesProgress:{
            progress.text = qsTr("Checking for updates ")+percent+"%";
        }
    }
    Label{
        id:progress
        anchors.centerIn: parent
        text:qsTr("Checking for updates")
        font.weight: Font.Bold
        font.pointSize: 12
    }

    PagePackageTable{
        id:updatesPackageTable
        visible: false
        anchors.fill: parent
    }
    Component.onCompleted: {
        Database.getUpdatesAsync()
    }
    Connections{
        target: transaction
        onFinished:{
            Database.getUpdatesAsync();
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
