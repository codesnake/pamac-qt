import QtQuick 2.12
import QPamac.Transaction 1.0
import QPamac.Database 1.0

import NotificationServices 1.0
import Qt.labs.platform 1.1
Item{
    objectName: "trayIcon"
    Timer{
        interval: Database.config.refreshPeriod*3600000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            Database.getUpdatesAsync();
        }
    }

    Connections{
        target: Database
        onUpdatesReady:{
            updatesNotification.updatesCount =  upds.getReposUpdates().length;
            if(updatesNotification.updatesCount!==0){
                updatesNotification.show();
            }
        }

    }
    Notification{
        property var updatesCount
        id: updatesNotification
        summary: qsTr("Updates available")
        body: updatesCount+" new update(s)"
        iconName: "system-software-update"
        actions: ["Show"]
        onActionClicked: {
            if(actionId=="show"){
                showUpdates();
            }
        }
    }
    SystemTrayIcon {
        icon.name: "system-software-install"
        visible: updatesNotification.updatesCount>0 || !Database.config.noUpdateHideIcon
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
        onActivated: showApp();
    }
    signal showUpdates();
    signal showApp();
    signal quitApp();
}
