import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import "../js/JSUtils.js" as Utils

Rectangle{
    Rectangle{
        SequentialAnimation on x{
            loops: Animation.Infinite
            running: progressBar.indeterminate
            NumberAnimation{
                duration: 1100
                easing.type: Easing.InOutCubic
                from: 0
                to:progressBar.width-progress.width
            }
            NumberAnimation{
                duration: 1100
                easing.type: Easing.InOutCubic
                to: 0
                from:progressBar.width-progress.width
            }
        }
        
        id:progress
        height: parent.height
        width: parent.indeterminate?parent.width*0.1:parent.value*parent.width/(parent.to-parent.from)
        color: systemPalette.highlight
        Behavior on width {
            NumberAnimation{
                easing.type: Easing.InOutQuart
            }
        }
    }
    color: systemPalette.mid
    id:progressBar
    property bool indeterminate
    property var value
    property var from:0
    property var to:1
    onIndeterminateChanged: {
        if(!indeterminate){
            progress.x = 0
        }
    }
}
