import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QPamac.History 1.0
import QPamac.Database 1.0
import "../../js/JSUtils.js" as JSUtils

Pane{
    SystemPalette{id:systemPalette}
    property var title: qsTr("Transaction history")
    id:historyDialog
    padding: 3
    anchors.fill: parent

    Pane{
        Label{
            text: qsTr("Filter options:")
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: -filters.padding
            anchors.topMargin: -filters.padding
        }
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height:  50
        id:filters
        padding: 10

        GridLayout{
            anchors.fill: parent
            RowLayout{

                Label{
                    id: packageFilterLabel
                    text: qsTr("Package name:")
                    padding: 5
                }
                TextArea{
                    Layout.fillWidth: true
                    width: 100
                    id: packageFilter
                }
            }
            RowLayout{
                Label{
                    id: actionChooserLabel
                    text: qsTr("Actions:")
                    padding: 5
                }

                CheckBox{
                    id: installedCheckbox
                    text:qsTr("Installed")
                    checked: true
                }
                CheckBox{
                    id: removedCheckbox
                    text:qsTr("Removed")
                    checked: true
                }
                CheckBox{
                    id: upgradedCheckbox
                    text:qsTr("Upgraded")
                    checked: true
                }
                CheckBox{
                    id: unknownCheckbox
                    text:qsTr("Unknown")
                    checked: true
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
                let result = table.history;

                if(packageFilter.text!="")
                    result = result.filter(value => value.name.includes(packageFilter.text));

                result = result.filter(value=>{
                                           switch(value.type){
                                               case "Installed":
                                               return installedCheckbox.checked
                                               case "Removed":
                                               return removedCheckbox.checked
                                               case "Upgraded":
                                               return upgradedCheckbox.checked
                                               case "Unknown":
                                               return unknownCheckbox.checked
                                           }
                                           return true
                                       });
                return result
            }
            onHistoryListChanged:{
                table.view.contentY=0
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
