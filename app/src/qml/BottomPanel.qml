import QtQuick 2.12
import QtQuick.Controls 2.12
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
    RowLayout{


        ItemDelegate{
            id:detailsButton
            checked:bottomPanel.state=="expanded"
            checkable: true
            Layout.fillWidth: true
            Column{

                anchors.fill: parent
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Label{
                    width: parent.width

                    text: transaction.started?transaction.action:(sysUpgrade?calcTotalSize():totalPending + " pending")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle{
                    Rectangle{
                        SequentialAnimation on x{
                            loops: Animation.Infinite
                            running: progressBar.indeterminate
                            NumberAnimation{
                                duration: 1100
                                easing.type: Easing.InOutCubic
                                from: 0
                                to:progressBar.width-progress.width
                            }
                            NumberAnimation{
                                duration: 1100
                                easing.type: Easing.InOutCubic
                                to: 0
                                from:progressBar.width-progress.width
                            }
                        }

                        id:progress
                        height: parent.height
                        width: parent.indeterminate?parent.width*0.1:parent.value*parent.width/(parent.to-parent.from)
                        color: systemPalette.highlight
                    }
                    height: 5
                    color: systemPalette.mid
                    id:progressBar
                    width: parent.width
                    property bool indeterminate:  transaction.indeterminate
                    property var value: transaction.progress
                    property var from:0
                    property var to:1
                }
            }
        }



        Button{
            id:cancelButton
            visible: bottomPanel.width>(width+applyButton.width+detailsButton.width)

            enabled: !transaction.started
            text:qsTr("Cancel")
            onClicked: {
                if(sysUpgrade){
                    stackView.pop();
                }

                toInstall = toRemove = toBuild = toLoad = [];
            }
        }
        anchors.margins: 5
        id:row
        width: parent.width

        Button{
            id:applyButton
            visible: bottomPanel.width>(width+cancelButton.width+detailsButton.width)
            enabled: !transaction.started
            text:qsTr("Apply")
            onClicked: {
                if(!sysUpgrade)
                    transaction.start(toInstall,toRemove,toLoad,toBuild,[],[]);
                else
                    transaction.startSysupgrade(true,false,[],[]);
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
                Pane{
                    padding: 0
                    SideBar{
                        width: toBuild.length>0?undefined:0
                        id:sideMenuPending
                        initialItem:SideMenuPending{

                            anchors{
                                left:parent.left
                                bottom: parent.bottom
                                top:parent.top
                            }
                        }
                    }

                    PackageList{

                        anchors.left: sideMenuPending.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
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
