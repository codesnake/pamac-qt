import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    title: "Transaction Details"
    objectName: "transactionDetailsPage"
    padding: 5
    Pane{
        background: Rectangle{
            color:systemPalette.base
            border.color: systemPalette.mid
        }
        clip: true
        anchors.fill: parent
        padding: 0
        ListView{
            anchors.fill: parent
            ScrollBar.vertical: ScrollBar{
                visible:true
            }
            model: transaction.details.split("\n");
            delegate: Text {
                text: modelData
            }
            onCountChanged: {
                positionViewAtEnd();
            }
        }
    }
}
