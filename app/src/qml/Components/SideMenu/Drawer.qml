import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0

StackView {
    id:drawerStack

    width: 170
    anchors.left: parent.left
    height: parent.height

    Connections{
        target: transaction
        onStartedChanged:{
            if(!started)
                drawerStack.currentItem.StackView.activating();
        }
    }
}
