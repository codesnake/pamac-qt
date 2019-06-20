import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import Pamac.History 1.0
import Pamac.Database 1.0
import "../js/JSUtils.js" as JSUtils

Window{
    function open(){
        visible=true;
    }
    title: qsTr("Transaction history")
    height: 400
    maximumWidth: 320
    width: maximumWidth
    id:historyDialog
    Pane{
        padding: 3
        anchors.fill: parent
        TableView{
            ScrollBar.vertical: ScrollBar{
                visible: true
            }

            property var columnWidths:[80,60,100,80]
            anchors.fill: parent
            model:HistoryModel{
                historyList: Database.getHistory()
            }
            columnWidthProvider:(column)=>{
                return columnWidths[column];
            }
            rowHeightProvider: 25
            reuseItems:false
            delegate: Pane{
                clip: true
                background: Rectangle{
                    border.color: systemPalette.mid
                    color: systemPalette.base
                }
                padding:0
                Label{
                    wrapMode: Text.WordWrap
                    anchors.fill: parent
                    text: modelData
                }
            }
        }
    }
}
