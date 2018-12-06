import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamac.PackageModel 1.0

Page {
    background: Rectangle{
        color:"white"
    }

    PackageModel{
        id:packageModel
        packageList: modelData
    }
    property var modelData

    TableView{
        id:table
        model:packageModel
    }
}
