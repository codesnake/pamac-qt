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
                color:systemPalette.alternateBase
            }

            readOnly: true
            id:area
            text: transaction.details
        }
    }
}
