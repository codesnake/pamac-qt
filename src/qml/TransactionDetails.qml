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
                if((flickable.height-area.height)>0)
                flickable.contentY=(flickable.height-area.height)
            }
            readOnly: true
            id:area
            text: transaction.details
        }
    }
}
