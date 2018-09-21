import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamaq.alpm.packageModel 1.0
import Pamaq.alpm.database 1.0

ScrollView {
    property var model
    anchors.fill: parent
    
    ListView {
        id:listView
        model: parent.model
        boundsBehavior: Flickable.StopAtBounds
        spacing: 5
        header: Row {
            spacing: 1
            function itemAt(index) { return repeater.itemAt(index)}
            
            Repeater {
                width: parent.width
                id: repeater
                model: ["State","Name", "Version","Repository","Size"]
                Row{
                    Label {
                        height: 23
                        id:headerBlock
                        text: modelData
                        width: listView.width/5-5
                        font.pixelSize: Qt.application.font.pixelSize * 1.2
                        padding: 3
                        background: Rectangle { color: "lightblue"}
                    }
                    MouseArea{
                        property int mouseglobalX;
                        height: parent.height
                        width:4
                        cursorShape: Qt.SizeHorCursor
                        onPositionChanged: {
                            headerBlock.width+=mouse.x
                            repeater.itemAt(index+1).headerBlock.width-=mouse.x
                        }
                    }
                }
            }
        }

        
        delegate: PackageItemDelegate {
            height: 50
            width: parent.width
        }
    }
}
