import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamaq.alpm.database 1.0
import Pamaq.alpm.packageModel 1.0

StackView {
    SystemPalette{id:systemPallette}
    background: Rectangle{color: systemPallette.window}
    anchors.left: parent.left
    width:Math.min(150,parent.width*0.3)
    height: parent.height
}
