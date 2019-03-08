import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.Database 1.0
import Pamac.AUR.PackageModel 1.0
import Pamac.Async 1.0
import QtQuick.Shapes 1.11
Page {
    contentHeight: table.contentHeight+header.height
    function columnWidth(column) {
        return columnWidths[column]==="exp"?
                    width-width/4:columnWidths[column];
    }
    property alias columnWidths: table.columnWidths
    background: Rectangle{
        color:systemPalette.base
    }
    Connections{
        target: transaction
        onFinished:{
            packageFutureWatcher.r=!packageFutureWatcher.r;
        }
    }
    AURPackageModel{

        id:packageModel
        onPackageListChanged: {
            packageFutureWatcher.reset();
            headerRepeater.sortColumn=1;
        }
    }

    property alias packageList:packageModel.packageList
    Rectangle{
        color: systemPalette.mid
        height: 20
        width: parent.width
        id: header
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
                model: ListModel{
                    id:headerModel
                    ListElement{
                        name:qsTr("State")
                    }
                    ListElement{
                        name:qsTr("Name")
                    }
                    ListElement{
                        name:qsTr("Version")
                    }
                    ListElement{
                        name:qsTr("Popularity")
                    }
                }
                ItemDelegate {
                    layer.enabled: true
                    layer.samples: 4
                    rightPadding: 0
                    clip: true
                    height: parent.height
                    id:headerBlock
                    text: name
                    width: columnWidth(index)
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
                        anchors.right: parent.right
                        property int mouseglobalX
                        height: parent.height
                        width:5
                        cursorShape: Qt.SizeHorCursor
                        onPositionChanged: {

                            if(headerBlock.width+mouse.x>0){
                                if(columnWidths[index]==="exp"){
                                    columnWidths[index]=headerBlock.width;
                                }

                                columnWidths[index]+=mouse.x;
                                columnWidthsChanged();
                            }
                            table.forceLayout();
                        }
                    }

                }


            }
        }
    }
    function selectAll(){
        table.selectAll();
    }

    PackageTable {
        columnWidthProvider:columnWidth

        anchors{
            top:header.bottom
            bottom: parent.bottom
            left:parent.left
            right: parent.right
        }
        id:table
        delegate:AURPackageDelegate {
            id: packageDelegate
        }

    }


}
