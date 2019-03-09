import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0

TableView{
    onWidthChanged: {
        forceLayout();
    }
    Connections{
        target: packageModel
        onPackageListChanged:{
            contentY=0;
            clearSelected();
        }
    }

    property var columnWidths: [50,"exp",45,65,65]



    ScrollBar.vertical: ScrollBar{
        visible: true
    }

    ScrollBar.horizontal: ScrollBar{
        visible: true
    }
    property var selectedRows: []

    function select(row){
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

    clip: true
    id:table
    model:packageModel
    boundsBehavior: Flickable.StopAtBounds



    reuseItems: false


}
