import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import NotificationServices 1.0
import "../../js/JSUtils.js" as Utils
import "./SideMenu" as SideMenu
import "./" as Components


Pane {
    background: Pane{
        padding: 0
        Rectangle{
            height: 1

            anchors{
                left: parent.left
                top:parent.top
                right: parent.right
            }
            color: systemPalette.mid
        }
    }
    state:{
        if((transaction.details!=="" || totalPending!=0) && detailsButton.checked){
            return "expanded";
        }

        return (transaction.started ||
                toInstall.length>0 ||
                toRemove.length>0 ||
                toBuild.length>0 ||
                toLoad.length>0 ||
                sysUpgrade) ?"opened":"hidden"
    }
    padding: 3
    property bool sysUpgrade: stackView.currentItem.objectName == "updatesPage"
                              && stackView.currentItem.updates!==undefined
                              && (stackView.currentItem.updates.getReposUpdates().length!==0
                                  || stackView.currentItem.updates.getAurUpdates().length!==0)

    property int totalPending: toInstall.length+toRemove.length+toLoad.length+toBuild.length



    function calcTotalSize(){
        var list = stackView.currentItem.updates.getReposUpdates();
        var totalSize = 0;
        for(var i =0;i<list.length;i++){
            totalSize += ignoreWhenUpdate.indexOf(list[i].name)==-1?list[i].downloadSize:0;
        }
        if(totalSize==0){
            return "";
        }

        return qsTr("Download size: ")+Utils.readableFileSize(totalSize,true);
    }

    id:bottomPanel
    property bool side:false

    states:[
        State{
            name:"expanded"
            PropertyChanges{
                target: bottomPanel
                height:parent.height
            }
        },

        State{

            name:"opened"
            PropertyChanges {
                target: bottomPanel
                height:row.height+padding*2
            }
        },
        State{
            name:"hidden"
            PropertyChanges {
                target: bottomPanel
                height:0
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to:"*"
            animations:[
                NumberAnimation{
                    duration: 350
                    easing.type: Easing.InOutQuad
                    properties: "height"
                }
            ]
        }
    ]
    DesktopProgress{
        visible: transaction.started
        progress: transaction.progress
        indeterminate: transaction.indeterminate
    }

    RowLayout{
        ItemDelegate{
            id:detailsButton
            checked:bottomPanel.state=="expanded"
            checkable: true
            Layout.fillWidth: true
            padding: 5
            RowLayout{
                height: parent.height
                anchors.fill: parent
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Column{
                    Layout.fillWidth: true
                    Label{
                        width: parent.width

                        text: transaction.started?transaction.action:(sysUpgrade?calcTotalSize():totalPending + " pending")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Components.ProgressBar {
                        value: transaction.progress
                        indeterminate: transaction.indeterminate
                        id: progressBar
                        height: 6
                        width: parent.width
                    }
                }
                Image {
                    Layout.preferredHeight: parent.height/1.5
                    Layout.preferredWidth: parent.height/1.5
                    rotation: bottomPanel.state=="expanded"?270:90
                    Behavior on rotation {
                        NumberAnimation{}
                    }
                    source: "image://icon/go-previous"
                }
            }

        }



        Button{
            id:cancelButton

            enabled: !(transaction.started ||  (transaction.details.length!=0 && totalPending==0))
            text:qsTr("Cancel")
            onClicked: {
                if(sysUpgrade){
                    stackView.pop();
                }

                toInstall = [] ;
                toRemove = [];
                toBuild = [];
                toLoad = [];
            }
        }
        anchors.margins: 5
        id:row
        width: parent.width

        Button{
            id:applyButton
            enabled: !(transaction.started ||  (transaction.details.length!=0 && totalPending==0))
            text:qsTr("Apply")
            onClicked: {
                let success = false;
                if(!sysUpgrade)
                    success = transaction.run(toInstall,toRemove,toLoad,toBuild,[],[]);
                else
                    success = transaction.runSysupgrade(true);

                if (success){
                    mainWindow.clear();
                }
            }

        }
    }
    Loader{
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            top:row.bottom
        }

        property list<Component> pages:[
            Component{
                id:pendingComponent
                Pendings {
                }
            },
            Component{
                id:detailsComponent
                TransactionDetails{
                }
            }

        ]

        sourceComponent:((transaction.details!=="" && transaction.started) || (totalPending==0 && !transaction.started))?detailsComponent:pendingComponent
    }
    onTotalPendingChanged: {
        if(transaction.details.length!=0){
            if(totalPending!=0){
                transaction.details = "";
            }
        }
    }
    Connections{
        target: transaction
        onFinished:{
            if(state!="expanded"){
                transaction.details = "";
            }
        }
    }
}
