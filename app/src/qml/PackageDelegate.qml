import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import QPamac.AUR.PackageModel 1.0
import QPamac.Package 1.0
import "./" as PamacQt
import "../js/JSUtils.js" as JSUtils

LoaderDelegate{
    background: Rectangle{
        
        width: packageDelegate.width+5
        property var hovered:hoveredRow==row
        color: {
            if(highlighted){
                return systemPalette.highlight;
            }
            if(hovered || packageDelegate.containsMouse)
            {
                return systemPalette.midlight;
            }
            return systemPalette.base;
        }
    }
    
    
    onContainsMouseChanged: {
        if(containsMouse)
            hoveredRow=row
        else if(row==hoveredRow)
            hoveredRow=-1
    }
    onDoubleClicked: {
        if( list.packageList.every(value=>JSUtils.qmlTypeOf(value,"LibQPamac::Package")))
            stackView.push("PagePackageInfo.qml",{pkg:Database.getPkgDetails(name,appName,false)})
        else
            stackView.push("PageAURPackageInfo.qml",{packageFuture:Database.getAurPkgDetails(name)})
    }
    onClicked: {
        list.selectedRows = [];
        list.selectRow(row);
    }
    
    
    
    
    id: packageDelegate
    
    property bool highlighted:list.isSelected(row)
    
    
    function packageAction(){
        
        let el;
        if(list.packageList.every(value=>JSUtils.qmlTypeOf(value,"LibQPamac::Package")))
            if(installedVersion!=""){
                el = toRemove.indexOf(name);
                if(el!==-1){
                    toRemove.splice(el,1)
                    toRemoveChanged();
                }
                else{
                    toRemove.push(name);
                    toRemoveChanged();
                }
            } else{
                el = toInstall.indexOf(name);
                if(el!==-1){
                    toInstall.splice(el,1)
                    toInstallChanged();
                }
                else{
                    toInstall.push(name);
                    toInstallChanged();
                }
                
            }
        else{
            if(installedVersion!=""){
                el = toRemove.indexOf(name);
                if(el!==-1){
                    toRemove.splice(el,1)
                    toRemoveChanged();
                }
                else{
                    toRemove.push(name);
                    toRemoveChanged();
                }
            } else{
                el = toBuild.indexOf(name);
                if(el!==-1){
                    toBuild.splice(el,1)
                    toBuildChanged();
                }
                else{
                    toBuild.push(name);
                    toBuildChanged();
                }
                
            }
        }
    }
    
    
    
    function isPending(){
        if(installedVersion!=""){
            if(toRemove.indexOf(name)!=-1){
                return true
            }
            
            return false
        }
        if(JSUtils.qmlTypeOf(modelData,"LibQPamac::Package")){
            if(toInstall.indexOf(name)!=-1){
                return true
            } else {
                return false
            }
        }
        else{
            if(toBuild.indexOf(name)!=-1){
                return true
            } else {
                return false
            }
        }
        
    }
    columns: {
        if(stackView.currentItem.objectName == "updatesPage"){
            return JSUtils.qmlTypeOf(list.model,"LibQPamac::PackageModel")?updateRepoColumns:updateAurColumns
        }

        return JSUtils.qmlTypeOf(list.model,"LibQPamac::PackageModel")?repoColumns:aurColumns
    }
    property var repoColumns: [
        cellsPool.name,cellsPool.version,cellsPool.repo,cellsPool.size,cellsPool.installButton
    ]
    property var updateRepoColumns: [
        cellsPool.name,cellsPool.version,cellsPool.repo,cellsPool.downloadSize,cellsPool.updateButton
    ]
    property var aurColumns: [
        cellsPool.name,cellsPool.version,cellsPool.buildButton
    ]
    property var updateAurColumns: [
        cellsPool.name,cellsPool.version,cellsPool.buildButton
    ]
    Item {
        id: cellsPool
        property var name:Component{
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
                    source:(JSUtils.isAccessible(iconUrl) && iconUrl.toString().length)?Qt.resolvedUrl("file://"+iconUrl):"image://icons/package-x-generic"
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
        }
        property var version: Component{
            Label{
                clip: true
                text:version
            }
        }
       property var repo: Component{
            Label{
                clip: true
                text:repo
            }
        }
        property var size: Component{
            Label{
                clip: true
                text:size.toString()
            }
        }
        property var installButton: Component{
            Item {
                Button{
                    
                    checkable: true
                    anchors.fill: parent
                    anchors.margins: 5
                    text: installedVersion!=""?"Remove":"Install"
                    checked: isPending()
                    onClicked: packageAction()
                }
            }
        }
        property var downloadSize: Component{
            Label{
                clip: true
                text:downloadSize.toString()
            }
        }
        property var updateButton: Component{
            Item {
                Button{
                    checkable: true
                    anchors.fill: parent
                    anchors.margins: 5
                    text: "Update"
                    checked:  ignoreWhenUpdate.indexOf(name)!=undefined
                    onCheckedChanged: checked?ignoreWhenUpdate.push(name):ignoreWhenUpdate = ignoreWhenUpdate.filter(value=>value!==name)
                }
            }
        }

        property var buildButton: Component{
            Item {
                Button{
                    
                    checkable: true
                    anchors.fill: parent
                    anchors.margins: 5
                    text: installedVersion!=""?"Remove":"Build"
                    checked: isPending()
                    onClicked: packageAction()
                }
            }
        }
    }
    
}