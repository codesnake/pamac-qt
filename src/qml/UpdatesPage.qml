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
            updatesPackageTable.packageList = upList;
            if(upList.length===0){
                updates=undefined;
                progress.text = qsTr("System is up to date");
            }

        }
        onGetUpdatesProgress:{
            progress.text = qsTr("Checking for updates ")+percent+"%";
        }
    }
    Column{
        width: implicitWidth
        height: implicitHeight
        spacing: 1
        anchors.centerIn: parent
        BusyIndicator{
            anchors.horizontalCenter: parent.horizontalCenter
            visible: updates===undefined
            height: width
            width: progress.paintedWidth*0.5
            running: true
        }

        Label{
            visible: updates===undefined
            id:progress
            text:qsTr("Checking for updates")
            font.weight: Font.Bold
            font.pointSize: 12
        }
    }
    PagePackageTable{
        id:updatesPackageTable
        visible: packageList.length>0
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
