import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import "../js/JSUtils.js" as Utils
import "./" as PamacQt

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
            totalSize += ignoreWhenUpdate.indexOf(list[i].name)!=-1? list[i].downloadSize:0;
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
            MouseArea{
                anchors.fill: parent
                visible: sysUpgrade
                enabled: sysUpgrade
                hoverEnabled: true
            }

            id:detailsButton
            checked:bottomPanel.state=="expanded"
            checkable: true
            Layout.fillWidth: true
            padding: 5
            Column{

                anchors.fill: parent
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Label{
                    width: parent.width

                    text: transaction.started?transaction.action:(sysUpgrade?calcTotalSize():totalPending + " pending")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                PamacQt.ProgressBar {
                    id: progressBar
                    height: 6
                    width: parent.width
                }
            }
        }



        Button{
            id:cancelButton
            visible: bottomPanel.width>(width+applyButton.width+detailsButton.width)

            enabled: !(transaction.started ||  (transaction.details.length!=0 && totalPending==0))
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
            enabled: !(transaction.started ||  (transaction.details.length!=0 && totalPending==0))
            text:qsTr("Apply")
            onClicked: {
                if(!sysUpgrade)
                    transaction.start(toInstall,toRemove,toLoad,toBuild,[],[]);
                else
                    transaction.startSysupgrade(true,false,ignoreWhenUpdate,[]);
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
                        packageList: Database.findPackagesByName(toInstall.concat(toRemove))
                    }
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
        else if(totalPending==0){
            detailsButton.checked=false;
        }
    }
}
