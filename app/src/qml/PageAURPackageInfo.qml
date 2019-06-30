import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.Package 1.0
import QPamac.Database 1.0
import QPamac.Async 1.0
import QtQuick.Layouts 1.3
import "../js/JSUtils.js" as Utils

Page {
    FutureWatcher{
        id:detailsFutureWatcher

        onFinished: {
            pkg=result
        }
    }


    property alias packageFuture:detailsFutureWatcher.future
    objectName: "packageInfoPage"
    Connections{
        target: transaction
        onFinished:{
            packageFuture=Database.getAURPkgDetails(pkg.name);
        }
    }

    SideBar {
        background: Rectangle {
            color: systemPalette.alternateBase
        }

        id: drawer
        initialItem: SideMenuAURPackageInfo {
        }
    }
    title: pkg.name
    property var pkg
    id: aurPackageInfo
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
            source: "image://icons/package-x-generic"
        }

        Label {
            id: label
            height: 17
            text: pkg.name
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
            text: qsTr("Build")
            checkable: true

            checked: aurPackageInfo.state == "installedState" ? toRemove.indexOf( pkg.name) > -1 : toBuild.indexOf(pkg.name) > -1
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            onClicked: {
                if(state!="installedState"){
                    if(button.checked){
                        toBuild.push(pkg.name);
                        toBuildChanged();
                    }
                    else {
                        toBuild.splice(toBuild.indexOf(pkg.name),1);
                        toBuildChanged();
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

                    Grid {
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        id: grid
                        spacing: 5
                        rows: 6
                        columns: 2
                        height: implicitHeight

                        Label {
                            visible: pkg.url.toString()!==""
                            id: label2
                            x: 30
                            text: qsTr("URL:")
                            font.weight: Font.Bold
                            font.bold: true
                        }
                        Label {
                            id: label9
                            y: -5
                            text: "<a href=%1>%1</a>".arg(pkg.url)
                            textFormat: Text.RichText
                            onLinkActivated: Qt.openUrlExternally(link)

                        }
                        Label {
                            visible:  pkg.licenses.join(", ")!==""
                            id: label3
                            x: 30
                            text: qsTr("Licenses:")
                            font.bold: true
                            font.weight: Font.Bold
                        }
                        Label {
                            id: label10
                            y: -5
                            text: pkg.licenses.join(", ")
                        }
                        Label {
                            visible: pkg.maintainer!==""
                            id: label4
                            x: 30
                            text: qsTr("Maintainer:")
                            font.bold: true
                            font.weight: Font.Bold
                        }
                        Label {
                            id: label11
                            y: -5
                            text: pkg.maintainer
                        }


                        Label {
                            visible: pkg.firstSubmitted!==""
                            id: label5
                            x: 30
                            text: qsTr("First Submitted:")
                            font.weight: Font.Bold
                        }
                        Label {
                            id: label12
                            y: -5

                            text: pkg.firstSubmitted
                            textFormat: Text.RichText
                        }
                        Label {
                            id: label6
                            x: 30
                            text: qsTr("Last Modified:")
                            font.weight: Font.Bold
                        }
                        Label {
                            id: label13
                            y: -5
                            text: pkg.lastModified
                        }
                        Label {
                            id: label18
                            x: 30
                            text: qsTr("Popularity:")
                            font.weight: Font.Bold
                        }

                        Label {
                            id: label17
                            y: -5
                            text: pkg.popularity.toString()
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
                        id: label19
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
                        id: label20
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
            Pane{
                id:buildFilesPage
                anchors.fill: parent
                property var buildFolder
                property var buildFiles
                FutureWatcher{

                    future: Database.cloneBuildFiles(pkg.name,true);
                    onFinished: {
                        if(result!==""){
                            buildFilesPage.buildFolder = result
                            tabsWatcher.future = transaction.getBuildFiles(pkg.name);
                        }
                    }
                }
                FutureWatcher{
                    id:tabsWatcher

                    onFinished: {
                        buildFilesPage.buildFiles = {}
                        buildFileTabsRepeater.model=result;
                        for(var el = 0;el<result.length;el++){
                            buildFilesPage.buildFiles[result[el]]=Utils.readFile("file://"+buildFilesPage.buildFolder+'/'+result[el])
                        }
                        buildFilesPage.buildFilesChanged();
                    }
                }
                TabBar{
                    clip: true
                    height: contentHeight
                    id:buildFileTabBar
                    anchors.top: parent.top

                    anchors.right: parent.right
                    anchors.left: parent.left
                    Repeater{
                        height: tabButton.height
                        id:buildFileTabsRepeater
                        delegate: TabButton{

                            id:tabButton
                            text: modelData

                        }
                    }
                }
                ScrollView {
                    anchors.top: buildFileTabBar.bottom
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    clip: true
                    contentHeight: textArea.implicitHeight
                    TextArea {
                        background: Rectangle{
                            color: systemPalette.alternateBase
                        }
                        id: textArea
                        text:buildFilesPage.buildFiles[buildFileTabBar.currentItem.text]
                        onTextChanged: {
                            buildFilesPage.buildFiles[buildFileTabBar.currentItem.text]=text
                        }

                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
