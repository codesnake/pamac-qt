import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import Pamac.alpm.config 1.0
import Pamac.alpm.database 1.0
import Pamac.alpm.packageModel 1.0

Item {
    property var config
    id: item1
    anchors.fill: parent

    TabBar {
        id: tabBar
        currentIndex: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        TabButton {
            id: tabButton
            text: qsTr("General")
        }

        TabButton {
            id: tabButton1
            text: qsTr("Official repositories")
        }

        TabButton {
            id: tabButton2
            text: qsTr("AUR")
        }

        TabButton {
            id: tabButton3
            text: qsTr("Cache")
        }
    }

    StackLayout {
        id: stackLayout
        currentIndex: tabBar.currentIndex
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: tabBar.bottom
        anchors.topMargin: 0


        Pane{

            padding: 6
            GridLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                id: gridLayout
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                columns: 2
                Layout.preferredWidth: parent.width

                Label {
                    id: label
                    text: qsTr("Remove unrequired dependecies")
                }

                CheckBox {
                    checked: config.recurse
                    id: checkBox
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Label {
                    id: label1
                    text: qsTr("Check available disk space")
                }

                CheckBox {
                    checked: Database.checkspace
                    id: checkBox1
                    tristate: false
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Label {
                    id: label2
                    text: qsTr("Check for updates")
                }

                CheckBox {
                    checked: config.downloadUpdates
                    id: checkBox2
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }
            }

            Item {
                id: row
                anchors.top: gridLayout.bottom
                anchors.topMargin: 6
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                Layout.fillWidth: true

                Label {
                    id: label3
                    height: spinBox.height
                    text: qsTr("How often to check updates, value in hours:")
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: spinBox.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                }
                SpinBox {
                    id: spinBox
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                CheckBox {
                    checked: config.downloadUpdates
                    id: checkBox3
                    x: 0
                    text: qsTr("Automatically download updates")
                    anchors.top: spinBox.bottom
                    anchors.topMargin: 6
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                }

                CheckBox {
                    checked: config.noUpdateHideIcon
                    id: checkBox4
                    x: 0
                    text: qsTr("Hide tray icon when no update available")
                    anchors.top: checkBox3.bottom
                    anchors.topMargin: 6
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Label {
                    id: label4
                    x: 8
                    text: qsTr("Ignore upgrades for:")
                    anchors.top: checkBox4.bottom
                    anchors.topMargin: 6
                }

                Pane{
                    padding: 0
                    anchors.top: label4.top
                    anchors.topMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.left: label4.right
                    anchors.leftMargin: 6
                    background: Rectangle{
                        color:"white"
                    }

                ListView {

                    id: listView
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                   anchors.fill: parent
                    delegate: Text{
                        width:parent.width
                        text: modelData


                    }
                    // @disable-check M222
                    model:Database.getIgnorePkgs()
                }
                }
            }
        }

        Pane {
            id: page
            Label {
                id: label5
                height: comboBox.height
                text: qsTr("Use mirrors from:")
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: comboBox.verticalCenter
            }

            ComboBox {
                id: comboBox
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
            }

            Button {
                id: button
                x: 528
                y: 58
                text: qsTr("Rerfresh mirror list")
                anchors.right: parent.right
                anchors.rightMargin: 0
            }
        }

        Pane {
            id: page1
            Rectangle {
                id: rectangle
                height: childrenRect.height + 20
                color: "#ff9100"
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                clip:true
                Label {
                    id: label6
                    text: qsTr("AUR is a community maintened repository so it presents potential risks and problems.\nAll AUR users should be familiar with the build process.")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    padding: 6
                    anchors.fill: parent
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                }
            }

            Label {
                id: label7
                height: checkBox5.height
                text: qsTr("Enable AUR support")
                verticalAlignment: Text.AlignVCenter
                anchors.left: parent.left
                anchors.leftMargin: 6
                anchors.top: checkBox5.top
                anchors.topMargin: 0
            }

            CheckBox {
                checked: config.enableAur
                id: checkBox5
                x: 465
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.top: rectangle.bottom
                anchors.topMargin: 16
            }

            CheckBox {
                id: checkBox6
                text: qsTr("Check for updates from AUR")
                anchors.top: label7.bottom
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 30
            }

            Label {
                id: label8
                height: comboBox1.height
                text: qsTr("Build directory:")
                verticalAlignment: Text.AlignVCenter
                anchors.top: comboBox1.top
                anchors.topMargin: 0
                anchors.left: checkBox6.left
                anchors.leftMargin: 0
            }

            ComboBox {
                id: comboBox1
                x: 500
                displayText: "(None)"
                anchors.top: checkBox6.bottom
                anchors.topMargin: 6
                anchors.right: parent.right
                anchors.rightMargin: 6
            }
        }

        Pane {
            id: page2
            Label {
                id: label9
                height: spinBox1.height
                text: qsTr("Number of versions of each package to keep in the cache:")
                verticalAlignment: Text.AlignVCenter
                anchors.top: spinBox1.top
                anchors.topMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 6
            }

            SpinBox {
                id: spinBox1
                x: 493
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.top: parent.top
                anchors.topMargin: 6
            }

            CheckBox {
                checked: config.cleanRmOnlyUninstalled
                id: checkBox7
                text: qsTr("Remove only the versions of uninstalled packages")
                anchors.top: label9.bottom
                anchors.topMargin: 6
                anchors.left: parent.left
                anchors.leftMargin: 30
            }

            Button {
                id: button1
                x: 500
                text: qsTr("Clean cache")
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.top: checkBox7.bottom
                anchors.topMargin: 6
            }
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:5;anchors_width:240;anchors_x:102;anchors_y:155}
}
 ##^##*/
