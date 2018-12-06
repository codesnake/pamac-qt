import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../js/JSUtils.js" as Utils

Rectangle {
    property bool sysUpgrade: stackView.currentItem.objectName == "updatesPage"

    property int totalPending: toInstall.length+toRemove.length

    function tryLockAndRun(val){
        if(transaction.getLock()){
            val.apply(this, arguments)
        } else{

        }
    }

    function calcTotalSize(){
        var list = stackView.currentItem.updates.getReposUpdates();
        var totalSize = 0;
        for(var i =0;i<list.length;i++){
            totalSize += list.at(i).size;
        }
        return Utils.readableFileSize(totalSize,true);
    }

    id:bottomPanel
    Rectangle{
        anchors{
            top: parent.top

        }
        height: 1
        width:parent.width
        color:systemPallette.highlight
    }

    states:[
        State{
            name:"opened"
            PropertyChanges {
                target: bottomPanel
                height:row.height
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

            text:"Details"
            onClicked: {
                if(stackView.currentItem.objectName!="transactionDetailsPage")
                    stackView.push("TransactionDetails.qml");
            }
        }
        Button{
            enabled: !transaction.started
            text:"Apply"
            onClicked: {
                if(transaction.getLock()){
                    if(!sysUpgrade)
                        transaction.start(toInstall,toRemove,toLoad,toBuild,[],[]);
                    else
                        transaction.startSysupgrade(true,false,[],[]);
                }

            }
        }
        Button{
            enabled: !transaction.started
            text:"Cancel"
            onClicked: {
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
