import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.alpm.database 1.0
import Pamac.alpm.packageModel 1.0

StackView {
    anchors.left: parent.left
    width:Math.min(150,parent.width*0.3)
    height: parent.height
    Rectangle{
        anchors.right: parent.right
        color:systemPallette.highlight
        height: parent.height
        width: 1
        z:10
    }
}
