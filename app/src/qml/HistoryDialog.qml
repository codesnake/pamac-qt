import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QPamac.History 1.0
import QPamac.Database 1.0
import "../js/JSUtils.js" as JSUtils

Pane{
    SystemPalette{id:systemPalette}
    property var title: qsTr("Transaction history")
    id:historyDialog
    padding: 3
    anchors.fill: parent
    Pane{
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height:  50
        id:filters
        GridLayout{
            anchors.fill: parent
            Row{

                Label{
                    id: packageFilterLabel
                    text: qsTr("Package name:")
                }
                TextArea{
                    anchors.verticalCenter: packageFilterLabel.verticalCenter
                    width: 100
                    id: packageFilter
                }
            }
        }
    }

    Table{
        id:table
        property var history
        anchors{
            top:filters.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        background: Rectangle{
            color:systemPalette.alternateBase
        }

        model:HistoryModel{
            historyList: {
                let result;
                if(packageFilter.text!="")
                    result = table.history.filter(value => value.name.includes(packageFilter.text));

                return result
            }
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
        Component.onCompleted: {
            table.history = Database.getHistory();
        }
    }
}
