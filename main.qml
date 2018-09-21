import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamaq.alpm.database 1.0
import Pamaq.alpm.packageModel 1.0
ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("Package manager")


    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: "\u25C0"
            font.pixelSize: Qt.application.font.pixelSize * 1.4
            enabled: stackView.depth > 1 || drawer.depth > 1
            visible: enabled
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                }
                else if(drawer.depth>1){
                    drawer.pop();
                }
            }
        }


        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
        TextArea {
            font.pixelSize: Qt.application.font.pixelSize * 1.1
            placeholderText: "Search..."
            anchors.right: parent.right
            anchors.rightMargin: 0.01*parent.width
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle{
                color:"white"
                implicitWidth: parent.parent.width * 0.3
            }
        }
    }

    StackView{
        id:stackView
        anchors.fill: parent
        initialItem: Page{
            title:"Packages"
            SideBar {
                id: drawer
                initialItem: MainSideMenu{
                    height: parent.height-updatesItem.height
                }
            }
                ItemDelegate {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: 45
                    id:updatesItem
                    text: qsTr("Updates")
                    width: parent.width
                    onClicked: {
                        stackView.push("Page2Form.ui.qml")
                    }
                }

            PagePackageTable {
                id: mainView

                modelData: Database.getInstalledApps()

                anchors.right:parent.right
                anchors.top:parent.top
                anchors.bottom:parent.bottom

                width: window.width-drawer.width
                clip:true
            }
        }
    }
}

