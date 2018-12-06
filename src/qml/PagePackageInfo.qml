import QtQuick 2.4
import Pamac.Database 1.0
PagePackageInfoForm {
    label9.onLinkActivated: {
        onLinkActivated: Qt.openUrlExternally(link)
    }
    Connections{
        target: transaction
        onFinished:{
            pkg=Database.getPkgDetails(pkg.name,pkg.appName);
        }
    }
    button1.onClicked: {

        if(button1.checked){
            toInstall.push(pkg.name);
            toInstallChanged();
        }
        else {
            toInstall.splice(toInstall.indexOf(pkg.name),1);
            toInstallChanged();
        }
    }
    button.onClicked: {


        if(state!="installedState"){
            if(button.checked){
                toInstall.push(pkg.name);
                toInstallChanged();
            }
            else {
                toInstall.splice(toInstall.indexOf(pkg.name),1);
                toInstallChanged();
            }
        }
        else{
            if(button.checked){
                toRemove.push(pkg.name);
                toRemoveChanged();
            }
            else {
                toRemove.splice(toRemove.indexOf(pkg.name),1);
                toRemoveChanged();
            }
        }
    }

}
