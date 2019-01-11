import QtQuick 2.4
import QtQuick.Controls 2.3
import Pamac.Package 1.0
import Pamac.Database 1.0
import QtQuick.Layouts 1.3

Page {
    objectName: "packageInfoPage"
    Connections{
        target: transaction
        onFinished:{
            pkg=Database.getPkgDetails(pkg.name,pkg.appName);
        }
    }

    SideBar {
        background: Rectangle {
            color: systemPallette.alternateBase
        }

        id: drawer
        initialItem: SideMenuPackageInfo {
        }
    }
    title: pkg.appName
    property var pkg
    id: rectangle
    state: pkg.installedVersion ? "installedState" : ""
    Item {
        id: item1
        width: parent.width - drawer.width
        height: parent.height
        anchors.right: parent.right
        Image {
            id: image
            width: 60
            height: 60
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            asynchronous: true

            source: pkg.iconUrl !== "" ? "file://" + pkg.iconUrl : "image://icons/package-x-generic"
        }

        Label {
            id: label
            height: 17
            text: pkg.appName ? pkg.appName + " (" + pkg.name + ") "
                                + pkg.installedVersion : pkg.name
            font.pixelSize: 14
            anchors.top: parent.top
            anchors.topMargin: 20
            font.weight: Font.Bold
            wrapMode: Text.WordWrap
            anchors.left: image.right
            anchors.leftMargin: 30
            anchors.right: button.left
            anchors.rightMargin: 30
            elide: Qt.ElideRight
        }

        Label {
            id: label1
            width: 0
            text: pkg.desc
            anchors.top: label.bottom
            anchors.topMargin: 10
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

            checked: rectangle.state == "installedState" ? toRemove.indexOf( pkg.name) > -1 : toInstall.indexOf(pkg.name) > -1
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            onClicked: {
                if(state!="installedState"){
                    if(button.checked){
                        toInstall.push(pkg.name);
                        toInstallChanged();
                    }
                    else {
                        toInstall.splice(toInstall.indexOf(pkg.name),1);
                        toInstallChanged();
                    }
                }
                else{
                    if(button.checked){
                        toRemove.push(pkg.name);
                        toRemoveChanged();
                    }
                    else {
                        toRemove.splice(toRemove.indexOf(pkg.name),1);
                        toRemoveChanged();
                    }
                }
            }
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

            checked: toInstall.indexOf(pkg.name) > -1
                     && rectangle.state == "installedState"

            onClicked: {
                if(button1.checked){
                    toInstall.push(pkg.name);
                    toInstallChanged();
                }
                else {
                    toInstall.splice(toInstall.indexOf(pkg.name),1);
                    toInstallChanged();
                }
            }
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
            Flickable {
                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                boundsBehavior: Flickable.StopAtBounds
                id: scrollView
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                contentHeight: infoColumn.implicitHeight
                Column {
                    id: infoColumn
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
                        width: height * (sourceSize.width / sourceSize.height)
                        height: visible ? 200 : 0
                        asynchronous: true
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
                            text: "<a href=%1>%1</a>".arg(pkg.url)
                            textFormat: Text.RichText
                            onLinkActivated: Qt.openUrlExternally(link)

                        }

                        Label {
                            id: label10
                            y: -5

                            text: pkg.licenses.join(", ")
                        }

                        Label {
                            id: label11
                            y: -5
                            text: pkg.repo
                        }

                        Label {
                            property var email
                            id: label12
                            y: -5

                            text: {
                                var re = /.*(<(\S+)>)/;


                                var result = re.exec(pkg.packager);
                                email=result[2];


                                return pkg.packager.replace(result[1],"<a href=\"mailto:%1\">%1</a>".arg(result[2]))
                            }
                            textFormat: Text.RichText
                            onLinkActivated: Qt.openUrlExternally("mailto:"+email)
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
            Flickable {
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {
                    active: true
                }
                clip: true

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

                            onClicked: {

                                stackView.push("PagePackageInfo.qml", {"pkg" : Database.getPkgDetails(modelData)});
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

                            onClicked: {

                                stackView.push("PagePackageInfo.qml",{"pkg" : Database.getPkgDetails(modelData.split(":")[0])});
                            }
                        }
                        model: pkg.optDepends
                    }
                }
            }

            ScrollView {
                clip: true
                contentHeight: textArea.implicitHeight
                TextArea {
                    background: Rectangle{
                        color: systemPallette.alternateBase
                    }
                    readOnly: true
                    id: textArea

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
