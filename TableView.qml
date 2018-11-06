import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.alpm.packageModel 1.0
import Pamac.alpm.database 1.0

ListView {

    currentIndex: -1
    ScrollBar.vertical: ScrollBar {
        active: true
    }
    anchors.fill: parent
    id:listView
    boundsBehavior: Flickable.StopAtBounds
    spacing: 1
    header: Row {
        function itemAt(index) { return repeater.itemAt(index)}

        Repeater {
            width: parent.width
            id: repeater
            model: ListModel{
                ListElement{
                    name:"State"
                    panelWidth: 30
                }
                ListElement{
                    name:"Name"
                    expandable: true
                }
                ListElement{
                    name:"Version"
                    panelWidth: 45
                }ListElement{
                    name:"Repository"
                    panelWidth: 65
                }ListElement{
                    name:"Size"
                    panelWidth:65
                }
            }
            Row{
                 clip: true
                Label {
                     clip: true
                    height: 23
                    id:headerBlock
                    text: name
                    width: (expandable)?listView.width-listView.width/4:panelWidth
                    font.pixelSize: Qt.application.font.pixelSize * 1.2
                    padding: 3
                    background: ToolButton{flat:true}
                }
                MouseArea{
                    property int mouseglobalX;
                    height: parent.height
                    width:3
                    cursorShape: Qt.SizeHorCursor
                    onPositionChanged: {
                        if(headerBlock.width+mouse.x>0){
                            headerBlock.width+=mouse.x
                        }
                    }
                }
                Component.onCompleted: {
                    console.log(name+" "+width)
                }
            }
        }
    }


    delegate: PackageItemDelegate {
        height: 45
        width: parent.width
    }
}
