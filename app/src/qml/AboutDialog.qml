import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    function open(){
        visible=true
    }

    id: dialog
    title: "About Pamac Qt"
    width: 300
    height: 350
    Pane {
        anchors.fill: parent
        id: item1
        Image {
            id: image
            width: 100
            height: 100
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            source: "image://icons/system-software-install"
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
            text: qsTr("A Qt5 frontend to the libpamac")
            anchors.top: text1.bottom
            anchors.topMargin: 25
            font.pointSize: 11
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            id: label1
            text: qsTr("Artem Grinev (aka LordTermor) for Manjaro Linux")
            anchors.top: label.bottom
            anchors.topMargin: 25
            font.pointSize: 8
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            wrapMode: Text.WordWrap
            id: label2
            text: qsTr("Version: ")+ Qt.application.version
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 25
            anchors.top: label1.bottom
            anchors.topMargin: 25
            font.pointSize: 8
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Button{
            text:qsTr("OK")
            onClicked: dialog.close();
            anchors{
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

}
