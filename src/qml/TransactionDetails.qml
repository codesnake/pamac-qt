import QtQuick 2.11
import QtQuick.Controls 2.12

Page {
    title: "Transaction Details"
    objectName: "transactionDetailsPage"
    ScrollView{
        id:scrollView
        anchors.fill: parent
        TextArea{
            background: Rectangle{
                color:systemPalette.base
            }
            onTextChanged: {
                Flickable.contentY=Math.min(-area.height,-(Flickable.contentHeight-area.height))
            }
            readOnly: true
            id:area
            text: transaction.details
        }
    }
}
