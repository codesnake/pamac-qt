import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0

StackView {
    id:drawerStack

    width: 170
    anchors.left: parent.left
    height: parent.height

    states:[
        State {
            name: "opened"
            PropertyChanges {
                target: drawerStack
                width:170

            }
        },
        State{
            name:"hidden"
            PropertyChanges {
                target: drawerStack
                width:0

            }
        }

    ]
    transitions: Transition {
        PropertyAnimation{
            property:"width"
        }

    }
}
