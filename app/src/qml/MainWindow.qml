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
        if(stackView.currentItem.objectName!=="updatesPage")
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
    property var ignoreWhenUpdate: []

    AboutDialog{
        id:aboutDialog
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

        onEmitAction: {
            NotificationService.setMessage(emitAction);
        }

        requestChooseProvider: (depend,list)=>{
                                   let objects = {"depend":depend,"lst":list,"provider":undefined}
                                   objects = DialogRunner.exec("qrc:/src/qml/ChooseProviderDialog.qml",objects)
                                   return objects["provider"]
                               }

        requestOptDepends: (pkgname,lst)=>{
                               let result = DialogRunner.exec("qrc:/src/qml/TransactionOptDependsDialog.qml",{"pkgName":pkgname,"opDeps":lst,"opted":undefined});
                               return objects["opted"];
                           }

        requestImportKey: (pkgname,key,owner)=>{
                              let objects = {"pkgName":pkgname,"key":key,"owner":owner,"ok":undefined}
                              objects = DialogRunner.exec("qrc:/src/qml/AskImportKeyDialog.qml",objects);
                              return objects["ok"];
                          }
        requestCommit: summary=>{
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
            anchors.bottom:parent.bottom
            height: parent.height
            width:Math.min(175,parent.width*0.3)
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
                                DialogRunner.exec("qrc:/src/qml/HistoryDialog.qml");
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
                        anchors.leftMargin: 0
                    }
                },
                State{
                    name:"hidden"
                    PropertyChanges {
                        target: sidePanel
                        anchors.leftMargin: -Math.min(175,sidePanel.parent.width*0.3)
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
                            properties: "anchors.leftMargin"
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
                    SequentialAnimation{

                        PauseAnimation {
                            duration: 300
                        }
                        NumberAnimation{
                            duration: 150
                        }
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
                width: parent.width
                id: drawer
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
                left:sidePanel.right
            }
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

                    onPackageListChanged: {
                        stackView.pop(this);

                    }
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
    Component.onCompleted: {
        //Init singleton
        NotificationService;

    }
}
