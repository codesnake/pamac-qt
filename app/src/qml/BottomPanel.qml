import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import "../js/JSUtils.js" as Utils

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
            color: systemPalette.windowText
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
            totalSize += list.at(i).downloadSize;
        }
        if(totalSize==0){
            return "";
        }

        return qsTr("Download size: ")+Utils.readableFileSize(totalSize,true);
    }

    id:bottomPanel


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
    RowLayout{
        anchors.margins: 5
        id:row
        width: parent.width
        Column{
            height: parent.height
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Label{
                width: parent.width

                text: transaction.started?transaction.action:(sysUpgrade?calcTotalSize():totalPending + " pending")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Layout.fillWidth: true
            ProgressBar{
                id:progressBar
                width: parent.width
                indeterminate: transaction.indeterminate
                value: transaction.progress
                from:0
                to:1
            }
        }

        Button{
            Layout.preferredWidth:  detailsButtonLabel.paintedWidth+detailsButtonIcon.width+padding*2
            padding: 5
            checkable: true
            id:detailsButton
            enabled: transaction.details!=="" || totalPending!=0
            Row{
                enabled: parent.enabled
                height: parent.height
                anchors.centerIn: parent
                Image {
                    enabled: parent.enabled
                    rotation: bottomPanel.state=="expanded"?270:90
                    id:detailsButtonIcon
                    width: height
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://icon/go-previous"
                    Behavior on rotation {
                        NumberAnimation{duration: 400}
                    }
                }
                Label{
                    enabled: parent.enabled
                    anchors.verticalCenter: parent.verticalCenter
                    id:detailsButtonLabel
                    text:transaction.details!==""?qsTr("Details"):qsTr("Pending")
                }
            }
        }
        Button{
            enabled: !transaction.started
            text:qsTr("Apply")
            onClicked: {
                tryLockAndRun(()=>{
                    if(!sysUpgrade)
                        transaction.start(toInstall,toRemove,toLoad,toBuild,[],[]);
                    else
                        transaction.startSysupgrade(true,false,[],[]);
                })
            }

        }

        Button{
            enabled: !transaction.started
            text:qsTr("Cancel")
            onClicked: {
                if(sysUpgrade){
                    stackView.pop();
                }

                toInstall = [];
                toRemove = [];
                toBuild=[];
                toLoad=[];
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
                Flickable{
                    Table{
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: item.contentHeight
//                        packageList: Database.getPending(toInstall,toRemove)
                    }
                }
            },
            Component{
                id:detailsComponent
                TransactionDetails{
                }
            }

        ]

        sourceComponent:transaction.details!==""?detailsComponent:pendingComponent
    }
    onTotalPendingChanged: {
        if(transaction.details.length!=0){
            if(totalPending!=0){
                transaction.details = "";
            }
        }
        else if(totalPending==0){
            detailsButton.checked=false;
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
