import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Pamac.alpm.database 1.0
import Pamac.alpm.packageModel 1.0
ToolBar {
    id: toolBar
    padding: 5
    contentHeight: toolButton.implicitHeight
    width:parent.width
    ToolButton {
        anchors.verticalCenter: parent.verticalCenter
        flat:true
        id: toolButton
        width: height
        icon.name: "go-previous-symbolic"
        font.pixelSize: Qt.application.font.pixelSize * 1.4
        enabled: stackView.depth > 1 || drawer.depth > 1
        visible: enabled
        onClicked: {
            if (stackView.depth > 1) {
                stackView.pop()
            }
            else if(drawer.depth>1){
                drawer.pop();
                mainView.modelData=Database.getInstalledApps()
            }
        }
    }
    
    
    Label {
        text: stackView.currentItem.title
        anchors.centerIn: parent
    }

    ToolButton {
        id: toolButton1
        anchors.right: parent.right
        width: height
        anchors.verticalCenterOffset: 0
        flat: true
        icon.name: "application-menu"
        font.pixelSize: Qt.application.font.pixelSize * 1.4
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            contextMenu.open();
        }

        Menu {
            y: toolButton1.height
            id: contextMenu
            Action { text: "Refresh databases" }
            Action { text: "View History" }
            Action { text: "Install local packages" }
            Action { text: "Preferences" }
            Action {
                text: "About"
                onTriggered: {
                    aboutDialog.open();
                }
            }
        }
    }
}
