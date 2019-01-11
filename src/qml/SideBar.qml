import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0

StackView {
    id:drawerStack
    states: [
        State{
            name:"opened"
            PropertyChanges {
                target: drawerStack
                width:Math.min(175,parent.width*0.3)
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
    transitions: [
        Transition {
            from: "*"
            to: "*"
            animations:[
                NumberAnimation{
                    duration: 200
                    easing.type: Easing.InOutQuad
                    properties: "width"
            }
            ]
        }]

    anchors.left: parent.left
    state: "opened"
    height: parent.height
}
