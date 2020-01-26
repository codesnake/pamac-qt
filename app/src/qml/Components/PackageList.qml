import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0

import QPamac.AUR.PackageModel 1.0
import QPamac.Package 1.0
import "./" as PamacQt
import "../../js/JSUtils.js" as JSUtils

Table{
    property var packageList: []
    showHeader: false

    model:modelLoader.item
    Loader{
        id: modelLoader
        sourceComponent: (JSUtils.isAccessible(packageList)
                          && list.packageList.every((item)=>
                                                    JSUtils.qmlTypeOf(item,"LibQPamac::AlpmPackage")))?
                              repoPackageModelComponent:aurPackageModelComponent
    }
    Component{
        id:repoPackageModelComponent
        PackageModel{
            id:repoPackageModel
            packageList: list.packageList
        }
    }
    Component{
        id: aurPackageModelComponent
        AURPackageModel{
            id:aurPackageModel
            packageList: list.packageList
        }
    }



    Column{
        width: implicitWidth
        height: implicitHeight
        anchors.centerIn: parent
        visible: JSUtils.isAccessible(packageList) && packageList.length===0
        spacing: 3
        Image {
            width: parent.width/2
            height: width
            source: "image://icons/package-x-generic"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label{
            id:progress
            text:qsTr("This category is empty")
            font.weight: Font.Bold
            font.pointSize: 12
        }
    }

    id:list
    property var hoveredRow:-1
    delegate: PackageDelegate {
        id: packageDelegate
        height: 25
    }
    clip:true
    onPackageListChanged: {
        opacity = 1;
        list.selectedRows = [];
        view.contentY=0;
    }
    Behavior on opacity {
        NumberAnimation{}
    }
}
