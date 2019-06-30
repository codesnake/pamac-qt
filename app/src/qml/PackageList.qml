import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import "./" as PamacQt
import "../js/JSUtils.js" as JSUtils

Table{
    property alias packageList:packageModel.packageList
    property alias packageListFuture:packageModelWatcher.future
    model: PackageModel{
        id:packageModel
        property var watcher: FutureWatcher{
            id:packageModelWatcher
            onFinished: {
                packageModel.packageList = result;
            }
        }
    }
    id:list
    property var hoveredRow:-1
    delegate: LoaderDelegate{
        onContainsMouseChanged: {
            if(containsMouse)
                hoveredRow=row
            else if(row==hoveredRow)
                hoveredRow=-1
        }
        onDoubleClicked: {
            stackView.push("PagePackageInfo.qml",{pkg:Database.getPkgDetails(name,appName,false)})
        }
        onClicked: {
            list.selectedRows = [];
            list.selectRow(row);
        }



        hovered: hoveredRow==row
        height: 25
        id: packageDelegate

        property bool highlighted:list.isSelected(row)
        color: highlighted?systemPalette.highlight:systemPalette.base


        function packageAction(){

            const arr = installedVersion!=""?toRemove:toInstall;

            if(arr.indexOf(name)!=-1){
                arr.filter(item => item !== name)
                toRemoveChanged();
                toInstallChanged();
            }
            else{
                arr.push(name);
                toRemoveChanged();
                toInstallChanged();
            }
        }



        function isPending(){

            if(installedVersion!=""){
                if(toRemove.indexOf(name)!=-1){
                    return true
                }

                return false
            }

            if(toInstall.indexOf(name)!=-1){
                return true
            } else {
                return false
            }

        }

        columns: [

            Component{
                Row{
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    height: parent.height
                    spacing: 5
                    Image{
                        anchors.verticalCenter: parent.verticalCenter
                        id:packageIcon
                        width: 25
                        height: width
                        source:iconUrl.toString().length?Qt.resolvedUrl("file://"+iconUrl):"image://icons/package-x-generic"
                    }
                    
                    Column{
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width-packageIcon.width-5
                        Label {
                            
                            width:parent.width
                            text:appName?appName+" ("+name+")":name
                            font.weight: Font.Bold
                            font.bold: true
                            
                            elide: Text.ElideRight
                        }
                        Label {
                            width:parent.width
                            text:desc
                            elide: Text.ElideRight
                        }
                    }
                }
            },
            Component{
                Label{
                    clip: true
                    text:version
                }
            },
            Component{
                Label{
                    clip: true
                    text:repo
                }
            },
            Component{
                Label{
                    clip: true
                    text:size.toString()
                }
            },
            Component{
                Item {
                    Button{
                        anchors.fill: parent
                        anchors.margins: 5
                        text: installedVersion!=""?"Remove":"Install"
                        checked: isPending()
                        onClicked: packageAction()
                    }
                }
            }
        ]
    }
    clip:true
    //                    onPackageListChanged: {
    //                        if(stackView.depth>1 && stackView.currentItem.objectName!="updatesPage"){
    //                            stackView.pop(this);
    //                        }
    //                    }
}
