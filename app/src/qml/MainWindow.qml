import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import NotificationService 1.0
import DialogRunner 1.0
import "./" as PamacQt
import "../js/JSUtils.js" as JSUtils

ApplicationWindow {
    function showUpdates(){
        stackView.push("UpdatesPage.qml");
    }


    onClosing: {
        if(transaction.started)
            close.accepted = false
    }

    function clear(){
        toInstall=[];
        toRemove=[];
        toBuild=[];
        toLoad=[];
    }

    property var toInstall: []
    property var toRemove: []
    property var toBuild: []
    property var toLoad: []

    AboutDialog{
        id:aboutDialog
    }

    HistoryDialog {
        id: historyDialog
    }

    Transaction{
        id:transaction
        database: Database
        onStartPreparing: {
            if(started){
                details = "";
            }
        }

        onFinished:{
            if(success){
                clear();
            }
        }
        Component.onDestruction: {
            transaction.quitDaemon();
        }
        onStartedChanged: {
            NotificationService.setVisible(started);
        }

        onProgressChanged: {
            NotificationService.setProgress(progress);
        }
        requestCommit: summary=>{
                           console.log(JSON.stringify(summary));
            let objects = {"summary":summary,"result":undefined}
            objects =  DialogRunner.exec("qrc:/src/qml/TransactionSummaryDialog.qml",objects);
            return objects["result"]
        }
    }

    id: mainWindow
    visible: true
    minimumWidth: 750
    minimumHeight: 450
    width: 950
    height: 550
    objectName: "mainWindow"
    title: qsTr("Pamac Qt - preview version")
    SystemPalette{id:systemPalette}

    Item{
        anchors.fill: parent

        focus: true
        Keys.forwardTo: searchPane
        Page{
            id:sidePanel
            anchors.top: parent.top
            anchors.left:parent.left
            anchors.bottom:bottomPanel.top
            height: parent.height
            header:ToolBar{
                padding: 5
                height: toolBar.height
                z:10
                ToolButton {
                    checkable: true;
                    id: toolButton1
                    anchors.left: parent.left
                    width: height
                    flat: true
                    icon.name: "application-menu"
                    font.pixelSize: Qt.application.font.pixelSize * 1.4
                    anchors.verticalCenter: parent.verticalCenter

//                    FileDialog{
//                        id:fileDialog
//                        title: qsTr("Install Local Packages")
//                        folder: shortcuts.home
//                        nameFilters: ["Alpm packages (*.pkg.tar.xz)"]
//                        onAccepted: {
//                            for(var i =0;i<fileUrls.length;i++){
//                                toLoad.push(fileUrls[i].toString());
//                            }

//                            tryLockAndRun(()=>{transaction.start([],[],toLoad,[],[],[])});

//                        }
//                    }
                    Menu {
                        onClosed: toolButton1.checked=false


                        visible: toolButton1.checked
                        y: toolButton1.height
                        id: contextMenu
                        Action {
                            text: "Refresh databases"
                            onTriggered: {
                                transaction.startSysupgrade(true,false,[],[]);

                            }
                        }
                        Action { text: "View History"
                            onTriggered: {
                                historyDialog.open();
                            }
                        }
                        Action {
                            text: "Install local packages"
                            onTriggered: {
                                fileDialog.visible = true;
                            }
                        }
                        Action {
                            text: "Preferences"
                            onTriggered: {
                                DialogRunner.exec(Qt.resolvedUrl("PreferencesDialog.qml"),{"transaction":transaction})
                            }
                        }
                        Action {
                            text: "About"
                            onTriggered: {
                                aboutDialog.open();
                            }
                        }
                    }
                }
                SearchPane {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: toolButton1.right
                    anchors.right: parent.right
                    height: parent.height-5
                    id: searchPane

                }
            }
            states: [
                State{
                    name:"opened"
                    PropertyChanges {
                        target: sidePanel
                        width:Math.min(175,parent.width*0.3)
                    }
                },
                State{
                    name:"hidden"
                    PropertyChanges {
                        target: sidePanel
                        width:0
                    }
                }
            ]
            transitions: [
                Transition {
                    from: "*"
                    to: "*"
                    animations:[
                        NumberAnimation{
                            duration: 200
                            easing.type: Easing.InOutQuad
                            properties: "width"
                        }
                    ]
                }]
            state: "opened"

            MenuItemDelegate{

                leftPadding: backButtonIcon.x+backButtonIcon.width
                id:backMenuItem
                anchors.top: parent.top
                anchors.topMargin: drawer.depth > 1?0:-height
                anchors.left: parent.left
                width: drawer.width
                text: qsTr("Back")
                Image {
                    id:backButtonIcon
                    width: height
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://icon/go-previous"
                }
                Behavior on anchors.topMargin {
                    NumberAnimation{
                    }
                }

                onClicked: {
                    if(drawer.currentItem.objectName=="sideMenuSearch"){
                        searchPane.text = "";
                    } else{
                        drawer.pop();
                    }
                }
            }
            SideBar {

                clip: true
                anchors.top: backMenuItem.bottom
                anchors.bottom: parent.bottom
                id: drawer
                width: parent.width
                initialItem: SideMenuMain{
                    height: parent.height
                }

            }

        }

        Page{

            anchors{
                right:parent.right
                top:parent.top
                bottom: parent.bottom
            }
            width: mainWindow.width-drawer.width
            header:PamacQt.ToolBar {
                id: toolBar
                height: 40

            }
            StackView{
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: -1
                    radius: 5
                    color: systemPalette.dark
                }

                id:stackView
                height: parent.height
                width: parent.width
                clip: true
                initialItem: PackageList {
                    property string title
                    id: mainView

                }


            }
        }

    }
    BottomPanel{
        id:bottomPanel
        anchors{
            bottom:parent.bottom
            left:parent.left
            right:parent.right
        }

    }
}
