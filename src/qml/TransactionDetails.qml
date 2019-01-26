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

            readOnly: true
            id:area
            font.pointSize: 10

            Connections{
                target: transaction
                onDetailsChanged:{
                    area.clear();
                    area.append(transaction.details);
                }
            }
        }
    }
    Component.onCompleted: {
        area.append(transaction.details);
    }
}
