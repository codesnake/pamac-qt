import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.PackageModel 1.0
import QPamac.Database 1.0
import QPamac.Async 1.0
import QtQuick.Shapes 1.11
Rectangle{

    property var model: undefined
    property var customWidths:({})

    id: header

    function getColumnWidth(index){

        let a = header.model.headerData(index, 0, 13); //13 is a Qt::SizeHint


        if(a!=="fill"){
            return a;
        } else{
            let fillCount = 0;
            let size = 0;
            for(let i = 0;i<header.model.columnCount; i++){
                let b = header.model.headerData(i, 0, 13); //13 is a Qt::SizeHint
                if(b!=="fill"){
                    if(i in customWidths){
                        b = customWidths[i];
                    }

                    size+=b;
                } else{
                    fillCount++;
                }
            }
            return header.width/fillCount - size;
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
            property int sortColumn: 1
            property int sortOrder: Qt.DescendingOrder
            height: parent.height
            width: parent.width
            model: header.model !== null && header.model.columnCount
            ItemDelegate {

                layer.enabled: true
                layer.samples: 4
                rightPadding: 0
                height: parent.height
                id:headerBlock
                text: header.model.headerData(index,0,0)
                width: customWidths[index]
                font.pixelSize: 12
                padding: 3
                
                onClicked: {
                    if(headerRepeater.sortColumn != index){
                        headerRepeater.sortColumn = index;
                    } else{
                        headerRepeater.sortOrder=!headerRepeater.sortOrder;
                    }
                    table.model.sort(index, headerRepeater.sortOrder);
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
                        let tmpIndex = index-1;

                        if(headerBlock.width+mouse.x>0){
                            
                            header.customWidths[tmpIndex]+=mouse.x;
                            header.customWidths[index]-=mouse.x;
                            header.customWidthsChanged();

                        }
                        table.forceLayout();
                    }
                }

            }
        }
    }
    onWidthChanged: {
        for(let i = 0;i<header.model.columnCount; i++){
            if(header.model.headerData(i, 0, 13)==="fill"){
                customWidths[i] = getColumnWidth(i);
            }
        }
        header.customWidthsChanged();
    }
    onModelChanged: {
        if(header.model!==null){
            for(let i = 0;i<header.model.columnCount; i++){
                header.customWidths[i]=header.getColumnWidth(i);
            }
            header.customWidthsChanged();
        }
    }
}
