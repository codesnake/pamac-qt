import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.History 1.0
import QPamac.Database 1.0
import "../js/JSUtils.js" as JSUtils

Pane{
    SystemPalette{id:systemPalette}
    property var title: qsTr("Transaction history")
    id:historyDialog
    padding: 3
    anchors.fill: parent
    Table{
        background: Rectangle{
            color:systemPalette.alternateBase
        }

        anchors.fill: parent
        model:HistoryModel{
            historyList: Database.getHistory()
        }

        delegate: Label{
            background: Rectangle{
                color: (row%2!==0)?systemPalette.alternateBase:systemPalette.mid
            }

            Rectangle{
                anchors{
                    top: parent.top
                    left:parent.left
                    right: parent.right
                }
                height:2
                color: (row%2===0)?systemPalette.alternateBase:systemPalette.mid
            }
            padding: 5
            wrapMode: Text.WordWrap
            text: modelData
        }
    }
}
