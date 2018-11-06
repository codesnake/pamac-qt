import QtQuick 2.4
import QtQuick.Controls 2.4

Item {
    anchors.fill: parent
    id: item1
    Image {
        id: image
        width: 100
        height: 100
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.horizontalCenter: parent.horizontalCenter
        source: "file:///usr/share/icons/breeze/mimetypes/64/package-x-generic.svg"
    }

    Label {
        id: text1
        text: qsTr("Pamac Qt")
        anchors.top: image.bottom
        anchors.topMargin: 25
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
    }

    Label {
        id: label
        text: qsTr("A Qt5 frontend to the libalpm")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 25
        anchors.top: text1.bottom
        anchors.topMargin: 25
        font.pointSize: 11
        anchors.horizontalCenter: parent.horizontalCenter
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2;anchors_height:100;anchors_width:100;anchors_x:270;anchors_y:103}
}
 ##^##*/
