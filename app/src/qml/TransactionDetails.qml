import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    title: "Transaction Details"
    objectName: "transactionDetailsPage"
    Flickable{
        id:scrollView
        anchors.fill: parent
        ScrollBar.vertical: ScrollBar{
            visible:true
        }
        contentHeight: area.paintedHeight
        contentWidth: area.paintedWidth
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
