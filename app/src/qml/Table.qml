import QtQuick 2.12
import QtQuick.Controls 2.4
import QPamac.PackageModel 1.0
import QPamac.Database 1.0
import QPamac.Async 1.0
import QtQuick.Shapes 1.11
Pane{
    padding: 0
    property var model
    property var view: table
    contentHeight: table.contentHeight

    background: Rectangle{
        color:systemPalette.base
    }
    id:tableRoot

    Header {
        visible: showHeader
        anchors{
            top:parent.top
            left:parent.left
            right:parent.right
            rightMargin: verticalScrollbar.width+header.model.columnCount
        }

        id: header
        color: systemPalette.mid
        height: 20
        model: tableRoot.model
        onWidthChanged: {
            for(let row =0;row<table.rows;row++) {
                for(let column =0;column<table.columns;column++) {
//                    table.contentItem.children[row*table.columns+column].width = header.getColumnWidth(column);
                    table.update();
                    table.forceLayout();

                }
            }
        }
    }
    property bool showHeader
    property alias delegate: table.delegate
    property var selectedRows: []

    function selectRow(row){
        var i = selectedRows.indexOf(row)
        if(i==-1){
            selectedRows.push(row);
            selectedRowsChanged();
        }
        else{
            selectedRows.slice(i,1)
            selectedRowsChanged();
        }
    }
    function selectAll(){
        for(var i =0;i<model.rowCount();i++){
            selectedRows.push(i);
        }
        selectedRowsChanged();
    }

    function isSelected(row){
        return selectedRows.indexOf(row)>-1
    }
    function clearSelected(){
        selectedRows = [];
    }

    TableView{
        columnSpacing: 1
        columnWidthProvider:(column)=>{
                                return header.getColumnWidth(column);
                            }
        anchors{
            top:header.bottom
            bottom: parent.bottom
            left:parent.left
            right: parent.right
            rightMargin: verticalScrollbar.width
            bottomMargin: horizontalScrollbar.height
        }
        id:table


        ScrollBar.vertical: verticalScrollbar

        ScrollBar.horizontal: horizontalScrollbar


        boundsBehavior: Flickable.StopAtBounds
        clip:true

        model:tableRoot.model
        reuseItems: true


    }
    ScrollBar{
        anchors {
            top: parent.top
            left: table.right
            bottom:horizontalScrollbar.top
        }
        id:verticalScrollbar
        visible: true
    }
    ScrollBar{
        height: table.contentWidth>table.width?undefined:0
        anchors{
            left:parent.left
            top:table.bottom
            right: verticalScrollbar.left
        }
        id:horizontalScrollbar
        visible: true
    }
}

