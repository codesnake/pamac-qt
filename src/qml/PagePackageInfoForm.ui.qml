import QtQuick 2.4
import QtQuick.Controls 2.3
import Pamac.Package 1.0
import Pamac.Database 1.0
import QtQuick.Layouts 1.3

Page {
    SideBar {
        id: drawer
        clip: true
        anchors.left: parent.left
        initialItem: SideMenuPackageInfo {
        }
    }
    font.pixelSize: Qt.application.font.pixelSize * 1.1
    title: pkg.appName
    property var pkg
    property alias button: button
    property alias button1: button1
    property alias label9: label9
    id: rectangle
    state: pkg.installedVersion ? "installedState" : ""
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
            asynchronous:true
            // @disable-check M126
            source: pkg.iconUrl != "" ? "file://" + pkg.iconUrl : "image://icons/package-x-generic"
        }

        Label {
            id: label
            y: 40
            height: 17
            text: pkg.appName ? pkg.appName : pkg.name
            wrapMode: Text.WordWrap
            anchors.left: image.right
            anchors.leftMargin: 30
            anchors.right: button.left
            anchors.rightMargin: 30
            elide: Qt.ElideRight
        }

        Label {
            id: label1
            y: 75
            width: 0
            text: pkg.desc
            wrapMode: Text.WordWrap
            anchors.left: image.right
            anchors.leftMargin: 30
            anchors.right: button.left
            anchors.rightMargin: 30
        }

        Button {
            enabled: !transaction.started && !button1.checked
            id: button
            height: 30
            text: qsTr("Install")
            checkable: true
            // @disable-check M222
            checked: rectangle.state=="installedState"?toRemove.indexOf(pkg.name)>-1:toInstall.indexOf(pkg.name)>-1
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 30
        }

        Button {
            enabled: !transaction.started && !button.checked
            id: button1
            height: 30
            text: qsTr("Reinstall")
            anchors.horizontalCenter: button.horizontalCenter
            anchors.top: button.bottom
            anchors.topMargin: 10
            visible: false
            checkable: true
            // @disable-check M222
            checked: toInstall.indexOf(pkg.name)>-1 &&  rectangle.state=="installedState"
        }



        StackLayout {
            id: infoStackLayout
            width: parent.width - 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            currentIndex: 0
            anchors.top: label1.bottom
            anchors.topMargin: 30
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Flickable{
                ScrollBar.vertical: ScrollBar{
                    active:true
                }

                boundsBehavior: Flickable.StopAtBounds
                id: scrollView
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                contentHeight: infoColumn.implicitHeight
                Column {
                    id:infoColumn
                    anchors.fill: parent
                    height: childrenRect.height
                    spacing: 6
                    Label {
                        id: label16
                        width: parent.width
                        height: paintedHeight
                        text: pkg.longDesc
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        wrapMode: Text.WordWrap
                    }

                    Image {
                        visible: source
                        id: image1
                        width: height*(sourceSize.width/sourceSize.height)
                        height: visible?200:0
                        asynchronous:true
                        fillMode: Image.PreserveAspectFit
                        source: pkg.screenshotUrl
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
                        height: implicitHeight

                        Label {
                            id: label2
                            x: 30
                            text: qsTr("URL:")
                            font.weight: Font.Bold
                            font.bold: true
                        }

                        Label {
                            id: label3
                            x: 30
                            text: qsTr("Licenses:")
                            font.bold: true
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label4
                            x: 30
                            text: qsTr("Repository:")
                            font.bold: true
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label5
                            x: 30
                            text: qsTr("Packager:")
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label6
                            x: 30
                            text: qsTr("Build date:")
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label7
                            x: 30
                            text: qsTr("Install date:")
                            visible: false
                            font.weight: Font.Bold

                        }

                        Label {
                            id: label8
                            x: 30
                            text: qsTr("Install reason:")
                            visible: false
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label9
                            y: -5

                            text: pkg.url
                            textFormat: Text.RichText
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
            }
            Flickable{
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar{
                    active: true
                }
                clip:true

                id: item2
                contentHeight: grid1.implicitHeight
                Grid {
                    id: grid1
                    spacing: 6
                    rows: 2
                    columns: 2
                    anchors.fill: parent
                    flow: Grid.TopToBottom

                    Label {
                        visible: pkg.depends.length > 0
                        id: label17
                        text: qsTr("Depends on:")
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
                                stackView.push("PagePackageInfo.qml", {
                                                   // @disable-check M222
                                                   "pkg": Database.getPkgDetails(
                                                              modelData)
                                               })
                            }
                        }
                    }
                    Label {
                        visible: pkg.optDepends.length > 0
                        id: label18
                        text: qsTr("Optional dependencies:")
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
                                stackView.push("PagePackageInfo.qml", {
                                                   // @disable-check M222
                                                   "pkg": Database.getPkgDetails(
                                                              modelData)
                                               })
                            }
                        }
                        model: pkg.optDepends
                    }
                }
            }

            ScrollView {
                clip:true
                contentHeight: textArea.implicitHeight
                TextArea {
                    readOnly: true
                    id: textArea
                    // @disable-check M222
                    text: Database.getPkgFiles(pkg.name).join("\n")
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    states: [
        State {
            name: "installedState"

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
                target: grid
                rows: 7
            }

            PropertyChanges {
                target: infoStackLayout
                currentIndex: 0
            }

            PropertyChanges {
                target: grid1
                rows: 2
            }
        }
    ]
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:8;anchors_y:64}D{i:16;anchors_x:95}D{i:17;anchors_x:95}
D{i:18;anchors_height:350}D{i:21;invisible:true}
}
 ##^##*/
