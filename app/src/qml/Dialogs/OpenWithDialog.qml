import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.Transaction 1.0
import QPamac.Database 1.0
import DialogRunner 1.0
import "../Components" as Components
ApplicationWindow {
    SystemPalette{
        id:systemPalette
    }

    width: 600
    height: 400
    visible: true
    objectName: "openWithDialog"
    title: qsTr("Install local package")
    function start(packageToLoadName){
        transaction.run([],[],[packageToLoadName])
    }
    Components.TransactionDetails{
        anchors.fill: parent
    }

    Transaction{
        id:transaction
        database: Database

        onFinished:{

        }
        Component.onDestruction: {
            transaction.quitDaemon();
        }

        requestChooseProvider: (depend,list)=>{
                                   let objects = {"depend":depend,"lst":list,"provider":undefined}
                                   objects = DialogRunner.exec("qrc:/src/qml/Dialogs/ChooseProviderDialog.qml",objects)
                                   return objects["provider"]
                               }

        requestOptDepends: (pkgname,lst)=>{
                               let result = DialogRunner.exec("qrc:/src/qml/Dialogs/TransactionOptDependsDialog.qml",{"pkgName":pkgname,"opDeps":lst,"opted":undefined});
                               return objects["opted"];
                           }

        requestImportKey: (pkgname,key,owner)=>{
                              let objects = {"pkgName":pkgname,"key":key,"owner":owner,"ok":undefined}
                              objects = DialogRunner.exec("qrc:/src/qml/Dialogs/AskImportKeyDialog.qml",objects);
                              return objects["ok"];
                          }
        requestCommit: summary=>{
                           let objects = {"summary":summary,"result":undefined}
                           objects =  DialogRunner.exec("qrc:/src/qml/Dialogs/TransactionSummaryDialog.qml",objects);
                           return objects["result"]
                       }
    }

}
