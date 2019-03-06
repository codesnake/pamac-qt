import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
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

    padding: 3
    property bool sysUpgrade: stackView.currentItem.objectName == "updatesPage"
                              && stackView.currentItem.updates!==undefined
                              && stackView.currentItem.updates.getReposUpdates().length!==0
                              && StackView.currentItem.updates.getAurUpdates().length!==0

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
            enabled: stackView.currentItem.objectName!="transactionDetailsPage" && transaction.started
            text:qsTr("Details")
            onClicked: {
                stackView.push("TransactionDetails.qml");
            }
        }
        Button{
            enabled: !transaction.started
            text:qsTr("Apply")
            onClicked: {
                tryLockAndRun(function(){
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

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
