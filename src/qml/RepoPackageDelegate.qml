import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0

Rectangle{
    implicitHeight: 45

    id:packageDelegate
    function actionFunc(name){
        if(toInstall.indexOf(name)!=-1){
            toInstall.splice(toInstall.indexOf(name),1);
            toInstallChanged();
        }
        else if(toRemove.indexOf(name)!=-1){
            toRemove.splice(toRemove.indexOf(name),1);
            toRemoveChanged();
        }
        else if(installedVersion!=""){
            toRemove.push(name);
            toRemoveChanged();
        } else{
            toInstall.push(name);
            toInstallChanged();
        }
    }
    
    function packageActionFunc(){

        if(table.selectedRows.indexOf(index)!=-1){
            for(var el in table.selectedRows){
                var name = packageModel.packageList.at(table.selectedRows[el]).name;
                actionFunc(name);
            }
        }
        else{
            actionFunc(model.name);
        }
        
    }
    Menu {
        y: itemMouseArea.mouseY
        x: itemMouseArea.mouseX
        id:contextMenu
        Action {
            text: (toInstall.indexOf(name)!=-1 || toRemove.indexOf(name)!=-1)?
                      qsTr("Remove selection"):
                      installedVersion!=""?qsTr("Remove"):qsTr("Install")
            onTriggered: packageDelegate.packageActionFunc()
        }
        Action {
            text: qsTr("Details")
            onTriggered: {
                stackView.push("PagePackageInfo.qml",{"pkg":Database.getPkgDetails(name,appName)})
            }
        }
    }
    
    property bool highlighted:table.isSelected(row)
    color: highlighted?systemPalette.highlight:systemPalette.base
    
    MouseArea{
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        id:itemMouseArea
        onClicked: {
            if(!(mouse.modifiers & Qt.ControlModifier)){
                table.clearSelected();
            }
            table.select(row);
            if(mouse.button & Qt.RightButton){
                contextMenu.open();
            }
        }
        onDoubleClicked: {
            stackView.push("PagePackageInfo.qml",{"pkg":Database.getPkgDetails(name,appName)})
        }
        
        anchors.fill: parent
    }
    
    Loader{
        anchors.fill: parent
        anchors.leftMargin: 2
        
        property list<Component> columns:[
            
            Component{
                Item {
                    Image {
                        MouseArea{
                            anchors.fill: parent
                            onClicked:packageDelegate.packageActionFunc()
                        }
                        
                        function getStateIcon(){
                            if(installedVersion!=""){
                                if(toRemove.indexOf(name)!=-1){
                                    return "package-remove"
                                }
                                
                                return "package-installed-updated"
                            }
                            
                            if(toInstall.indexOf(name)!=-1){
                                return "package-install";
                            } else {
                                return "package-available";
                            }
                        }
                        source: "image://icons/builtin/"+getStateIcon()
                        width: 15
                        height: width
                        anchors.centerIn: parent
                    }
                }
            },
            Component{
                Row{
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
            }
        ]
        Component.onCompleted: {
            sourceComponent=columns[column]
            
        }
    }
}
