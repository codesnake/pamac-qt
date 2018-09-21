import QtQuick 2.4
import QtQuick.Controls 2.3
import Pamaq.alpm.package 1.0
import Pamaq.alpm.database 1.0
import QtQuick.Layouts 1.3

Page {
    SideBar {
        id: drawer
        anchors.left: parent.left
        initialItem: PackageInfoSideMenu {
        }
    }
    font.pixelSize: Qt.application.font.pixelSize * 1.1
    title: pkg.appName
    property var pkg
    id: rectangle
    state: pkg.installedVersion ? "State1" : ""
    Item {
        id: item1
        width: parent.width - drawer.width
        height: parent.height
        anchors.right: parent.right
        Image {
            id: image
            width: 70
            height: 70
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 30
            // @disable-check M126
            source: pkg.iconUrl != "" ? "file://" + pkg.iconUrl : "file:///usr/share/icons/breeze/mimetypes/64/package-x-generic.svg"
        }

        Label {
            id: label
            y: 40
            height: 17
            text: pkg.appName ? pkg.appName : pkg.name
            anchors.left: image.right
            anchors.leftMargin: 30
            anchors.right: button.left
            anchors.rightMargin: 30
            elide: Qt.ElideRight
        }

        Label {
            id: label1
            y: 75
            text: pkg.desc
            anchors.left: image.right
            anchors.leftMargin: 30
            anchors.right: button.left
            anchors.rightMargin: 30
        }

        Button {
            id: button
            x: 292
            height: 30
            text: qsTr("Install")
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 30
        }

        Button {
            id: button1
            x: 292
            height: 30
            text: qsTr("Button")
            anchors.horizontalCenter: button.horizontalCenter
            anchors.top: button.bottom
            anchors.topMargin: 10
            visible: false
        }

        StackLayout {
            id: infoStackLayout
            width: parent.width - 60
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            currentIndex: 1
            anchors.top: label1.bottom
            anchors.topMargin: 30
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Column {
                id: column
                spacing: 6

                Label {
                    id: label16
                    height: paintedHeight
                    text: pkg.longDesc
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    wrapMode: Text.WordWrap
                }

                Grid {

                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    id: grid
                    flow: Grid.TopToBottom
                    spacing: 5
                    rows: 5
                    columns: 2

                    Label {
                        id: label2
                        x: 30
                        text: qsTr("URL")
                    }

                    Label {
                        id: label3
                        x: 30
                        text: qsTr("Licenses:")
                    }

                    Label {
                        id: label4
                        x: 30
                        text: qsTr("Repository:")
                    }

                    Label {
                        id: label5
                        x: 30
                        text: qsTr("Packager")
                    }

                    Label {
                        id: label6
                        x: 30
                        text: qsTr("Build date:")
                    }

                    Label {
                        id: label7
                        x: 30
                        text: qsTr("Install date:")
                        visible: false
                    }

                    Label {
                        id: label8
                        x: 30
                        text: qsTr("Install reason:")
                        visible: false
                    }

                    Label {
                        id: label9
                        y: -5

                        text: pkg.url
                    }

                    Label {
                        id: label10
                        y: -5
                        // @disable-check M222
                        text: pkg.licenses.join(", ")
                    }

                    Label {
                        id: label11
                        y: -5
                        text: pkg.repo
                    }

                    Label {
                        id: label12
                        y: -5
                        text: pkg.packager
                    }

                    Label {
                        id: label13
                        y: -5
                        text: pkg.buildDate
                    }

                    Label {
                        id: label14
                        y: -5
                        text: pkg.installDate
                        visible: false
                    }

                    Label {
                        id: label15
                        y: 13
                        text: pkg.installReason
                        visible: false
                    }
                }
            }

            Item {
                id: item2

                Grid {
                    id: grid1
                    rows: (pkg.depends.length > 0) + (pkg.optDepends.length > 0)
                    columns: 2
                    anchors.fill: parent
                    flow: Grid.TopToBottom

                    Label {
                        visible: pkg.depends.length > 0
                        id: label17
                        text: qsTr("Depends on:")
                    }

                    Label {
                        visible: pkg.optDepends.length > 0
                        id: label18
                        text: qsTr("Optional dependencies:")
                    }

                    ListView {
                        id: listView
                        x: 126
                        y: 30
                        width: 110
                        height: contentHeight
                        boundsBehavior: Flickable.StopAtBounds
                        model: pkg.depends
                        delegate: ItemDelegate {
                            height: 25
                            text: modelData
                            // @disable-check M223
                            onClicked: {
                                // @disable-check M222
                                stackView.push("PagePackageInfo.qml", {"pkg": Database.getPkgDetails(modelData)})
                            }
                        }
                    }

                    ListView {
                        id: listView1
                        boundsBehavior: Flickable.StopAtBounds
                        width: 110
                        height: contentHeight
                        delegate: ItemDelegate {
                            height: 25
                            text: modelData
                            // @disable-check M223
                            onClicked: {
                                // @disable-check M222
                                stackView.push("PagePackageInfo.qml", {"pkg": Database.getPkgDetails(modelData)})
                            }
                        }
                        model: pkg.optDepends
                    }
                }
            }

            Item {
                TextArea {
                    id: textArea
                    // @disable-check M222
                    text: Database.getPkgFiles(pkg.name).join("\n")
                    anchors.fill: parent
                }
            }
        }
    }
    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: button1
                text: qsTr("Reinstall")
                visible: true
            }

            PropertyChanges {
                target: button
                text: qsTr("Remove")
            }

            PropertyChanges {
                target: label15
                visible: true
            }

            PropertyChanges {
                target: label8
                visible: true
            }

            PropertyChanges {
                target: label14
                visible: true
            }

            PropertyChanges {
                target: label7
                visible: true
            }

            PropertyChanges {
                target: label
                width: 124
                height: 17
                anchors.rightMargin: 236
                renderType: Text.NativeRendering
            }

            PropertyChanges {
                target: label1
                anchors.rightMargin: 295
            }

            PropertyChanges {
                target: grid
                rows: 7
            }

            PropertyChanges {
                target: infoStackLayout
                currentIndex: 1
            }

            PropertyChanges {
                target: grid1
                rows: 2
            }
        }
    ]
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:16;anchors_x:95}D{i:17;anchors_x:95}
D{i:18;anchors_height:350}D{i:8;anchors_y:64}
}
 ##^##*/
