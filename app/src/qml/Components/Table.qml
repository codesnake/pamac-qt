import QtQuick 2.12
import QtQuick.Controls 2.4
import QPamac.PackageModel 1.0
import QPamac.Database 1.0

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
            rightMargin:(header.model !== null)? verticalScrollbar.width+header.model.columnCount:0
        }

        id: header
        color: systemPalette.mid
        height: showHeader?20:0
        model: tableRoot.model
        onWidthChanged: {
            for(let row =0;row<table.rows;row++) {
                for(let column =0;column<table.columns;column++) {
                    table.update();
                    table.forceLayout();

                }
            }
        }
    }
    property bool showHeader: true
    property alias delegate: table.delegate
    property var selectedRows: []

    function selectRow(row){
        var i = selectedRows.indexOf(row)
        if(i==-1){
            forceActiveFocus();
            selectedRows.push(row);
            selectedRowsChanged();
        }
        else{
            selectedRows.splice(i,1)
            selectedRowsChanged();
        }
    }
    Keys.onDownPressed: {
        let selectedRow = selectedRows[0];

        selectRow(selectedRow);
        selectRow(selectedRow+1);
    }

    Keys.onUpPressed: {
        let selectedRow = selectedRows[0];

        selectRow(selectedRow);
        selectRow(selectedRow-1);
    }
    focus: true

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
                                return header.customWidths[column];
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

