import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Pane {
    visible: false
    height: visible?implicitHeight:0
    RowLayout{
        width: parent.width
        ProgressBar{
            Layout.fillWidth: true

        }
        Button{
            enabled: false
            text:"Details"
        }
        Button{
            enabled: false
            text:"Apply"
        }
        Button{
            enabled: false
            text:"Cancel"
        }
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
