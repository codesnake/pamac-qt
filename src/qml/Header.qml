import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0
import Pamac.Async 1.0
import QtQuick.Shapes 1.11
Rectangle{

    property var model
    id: header
    function getColumnWidth(index){
        let a = header.model.headerData(index,0,13);
        if(a!="fill"){
            return a;
        } else{
            let size = 0;
            for(let i = 0;i<header.model.columnCount;i++){
                let b = header.model.headerData(i,0,13);
                if(b!="fill"){
                    size+=b;
                }
            }
            return header.width-size;
        }
    }
    Row{
        spacing: 1
        topPadding: 1
        bottomPadding: 1
        leftPadding: table.contentItem.x
        
        height: parent.height-2
        width: parent.width
        Repeater {
            id:headerRepeater
            property int sortColumn:1
            property int sortOrder: Qt.DescendingOrder
            height: parent.height
            width: parent.width
            model: header.model.columnCount
            ItemDelegate {

                layer.enabled: true
                layer.samples: 4
                rightPadding: 0
                height: parent.height
                id:headerBlock
                text: header.model.headerData(index,0,0)
                width: getColumnWidth(index)
                font.pixelSize: 12
                padding: 3
                
                onClicked: {
                    if(headerRepeater.sortColumn != index){
                        headerRepeater.sortColumn = index;
                    } else{
                        headerRepeater.sortOrder=(headerRepeater.sortOrder==Qt.DescendingOrder)?Qt.AscendingOrder:Qt.DescendingOrder;
                    }
                    packageModel.sort(index, headerRepeater.sortOrder);
                }
                Shape {
                    visible:  headerRepeater.sortColumn==index
                    smooth: true
                    id: tri
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: parent.height/4
                    height: parent.height-(parent.height/1.5)
                    width: height*1.2
                    antialiasing: true
                    rotation: headerRepeater.sortOrder==Qt.DescendingOrder?180:0
                    Behavior on rotation {
                        NumberAnimation{}
                    }
                    
                    ShapePath {
                        strokeColor: "transparent"
                        fillColor: systemPalette.text

                        
                        
                        startX: tri.width/2; startY: 0
                        PathLine { x: tri.width; y: tri.height }
                        PathLine { x: 0; y: tri.height }
                        PathLine { x: tri.width/2; y: 0 }
                    }
                }
                MouseArea{
                    visible: index!=0
                    z:5
                    anchors.left: parent.left
                    anchors.leftMargin: -5
                    property int mouseglobalX
                    height: parent.height
                    width:10
                    cursorShape: Qt.SizeHorCursor
                    onPositionChanged: {
                        
//                        if(headerBlock.width+mouse.x>0){
//                            if(getColumnWidths[index]==="fill"){
//                                getColumnWidths[index]=headerBlock.width;
//                            }
                            
//                            getColumnWidths[index]+=mouse.x;
//                            getColumnWidthsChanged();
//                        }
                        table.forceLayout();
                    }
                }
                
            }
            
            
        }
    }
}
