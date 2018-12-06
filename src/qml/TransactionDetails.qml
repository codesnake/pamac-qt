import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    objectName: "transactionDetailsPage"
    TextArea{
        readOnly: true
        anchors.fill: parent
        id:area
        text: transaction.details
    }
}
