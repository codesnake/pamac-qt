import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QPamac.History 1.0
import QPamac.Database 1.0
import "../js/JSUtils.js" as JSUtils

Window{
    function open(){
        visible=true;
    }
    title: qsTr("Transaction history")
    height: 400
    id:historyDialog
    Pane{
        padding: 3
        anchors.fill: parent
        Table{

            anchors.fill: parent
            model:HistoryModel{
                historyList: Database.getHistory()
            }

            delegate:
                Label{
                background: Rectangle{
                    border.color: systemPalette.alternateBase
                }
                    wrapMode: Text.WordWrap
                    text: modelData
                }
        }
    }
}
