import QtQuick 2.4
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

Dialog {
    id: dialog
    title: "About"
    standardButtons: Dialog.Ok
    height: 300
    width: 300
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
            source: "image://icons/package-x-generic"
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
            text: qsTr("A Qt5 frontend for the libpamac")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 25
            anchors.top: text1.bottom
            anchors.topMargin: 25
            font.pointSize: 11
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            id: label1
            text: qsTr("Artem Grinev (aka LordTermor) for Manjaro Linux")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 25
            anchors.top: label.bottom
            anchors.topMargin: 25
            font.pointSize: 8
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}