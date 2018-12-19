import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0

Page {
    background: Rectangle{
        color:systemPallette.base
    }

    PackageModel{
        id:packageModel
        packageList: modelData
    }
    property var modelData
    PackageTable {
        anchors.fill: parent
    }
    Connections{
        target: transaction
        onFinished: {
            packageModel.packageListChanged();
        }
    }
}
