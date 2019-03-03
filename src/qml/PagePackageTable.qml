import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0
import Pamac.Package 1.0
import Pamac.AUR.Package 1.0
import Pamac.Async 1.0
import QtQuick.Shapes 1.11
Loader{
    id:tableLoader

    property var packageListFuture

    FutureWatcher{
        id:packageFutureWatcher
        property bool r : true

        future: {r; return tableLoader.packageListFuture}
        onFinished: {
            tableLoader.packageList=result;
        }
    }
    property var packageList
    property list<Component> tables:[
        Component{
            id:pageRepoPackageTable
            PageRepoPackageTable {
            }
        },
        Component{
            id:pageAurPackageTable
            PageAURPackageTable{
            }
        }
    ]
    onLoaded: {
        item.packageList=packageList;
    }
    Binding{
        target: item
        property: "packageList"
        value: packageList
    }

    sourceComponent:{
        if(packageList!==undefined){
            if(packageList.packageType()==="AUR")
                return pageAurPackageTable;
            if (packageList.packageType()==="Repo")
                return pageRepoPackageTable;
        }
        return undefined;
    }

}
