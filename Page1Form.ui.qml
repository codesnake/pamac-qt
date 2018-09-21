import QtQuick 2.9
import QtQuick.Controls 2.2

Page {

    title: qsTr("Categories")

    ScrollView {
        anchors.fill: parent

        ListView {
            model: 20
            delegate: ItemDelegate {

                width: parent.width
                height: 50
                text: "Item " + index
            }
        }
    }
}
