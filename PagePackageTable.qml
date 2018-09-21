import QtQuick 2.0
import QtQuick.Controls 2.2
import Pamaq.alpm.packageModel 1.0

Page {
    AlpmPackageModel{
        id:packageModel
        packageList: modelData
    }
    property var modelData

    TableView{
        id:table
        model:packageModel
    }
}
