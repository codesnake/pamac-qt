import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window{
    function open(){
        visible=true
    }

    id: dialog
    title: "About Pamac Qt"
    width: 370
    height: 300
    Pane {
        anchors.fill: parent
        id: item1
        padding: 2

        Pane{
            anchors.topMargin: 4
            anchors.top: parent.top
            id:header
            height: implicitHeight
            width: implicitWidth
            Image {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                id: image
                width: 35
                height: 35
                source: "image://icons/system-software-install"
            }
            Column{
                anchors.left: image.right
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: text1
                    text: qsTr("Pamac-Qt")
                    font.bold: true
                    font.pixelSize: 20
                }

                Label {
                    wrapMode: Text.WordWrap
                    id: label2
                    text: qsTr("Version: ")+ Qt.application.version
                    font.pointSize: 8
                }
            }
        }
        ColumnLayout{
            id: column
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: header.bottom
            anchors.topMargin: 20
            TabBar{
                id:tabs
                TabButton {
                    text: qsTr("About")
                }
                TabButton {
                    text: qsTr("Developers")
                }
                TabButton {
                    text: qsTr("Special thanks")
                }
                TabButton {
                    text: qsTr("Libraries")
                }
            }
            StackLayout{
                currentIndex: tabs.currentIndex
                Label {

                    id: label
                    text: qsTr("A Qt5 frontend for libpamac\n(Work in progress)")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Label {
                    id: label1
                    text: qsTr("Artem Grinev\n(aka LordTermor)")
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true

                }
                Label{
                    text:qsTr("Guillaume Benoit\nPamac GTK and libpamac author")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Label{
                    text:qsTr("- Qt\n- libpamac\n- libqpamac")
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
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
