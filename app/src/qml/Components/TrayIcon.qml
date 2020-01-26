import QtQuick 2.12
import QPamac.Transaction 1.0
import QPamac.Database 1.0

import NotificationServices 1.0
import Qt.labs.platform 1.1
Item{
    property var updatesCount
    objectName: "trayIcon"
    Timer{
        interval: Database.config.refreshPeriod*3600000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            let upds = Database.getUpdates();
            updatesCount =  upds.getReposUpdates().length;
            if(updatesCount!==0){
                updatesNotification.show();
            }
        }
    }
    Notification{
        id: updatesNotification
        summary: qsTr("Updates available")
        body: {
            if(updatesCount>1){
                return updatesCount+" new updates";
            }
            return "New update";
        }

        iconName: "system-software-update"
        actions: ["Show"]
        onActionClicked: {
            if(actionId=="show"){
                showUpdates();
            }
        }
    }
    SystemTrayIcon {
        icon.name: updatesCount>0?"update-low":"update-none"

        visible: updatesCount>0 || !Database.config.noUpdateHideIcon
        menu: Menu{
            visible: false
            MenuItem{
                text: qsTr("Open package manager")
                onTriggered: showApp();
            }
            MenuItem{
                text: qsTr("Exit")
                onTriggered: quitApp();
            }
        }
        onActivated: updatesCount>0?showUpdates():showApp()
    }
    signal showUpdates();
    signal showApp();
    signal quitApp();
}
