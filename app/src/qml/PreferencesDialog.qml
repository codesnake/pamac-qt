import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Config 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0

import "../js/JSUtils.js" as Utils
Page{
    title: qsTr("Preferences")
    signal dialogAccepted
    signal dialogRejected
    signal dialogApplied
    signal close
    SystemPalette{
        id:systemPalette
    }

    property Transaction transaction

    property var settingsState: {
        "pamac":{},
        "alpm":{}
    }
    function applySettings(){
        if(Object.entries(settingsState["pamac"]).length !== 0)
            transaction.startWritePamacConfig(settingsState["pamac"]);
        if(Object.entries(settingsState["alpm"]).length !== 0)
            transaction.startWriteAlpmConfig(settingsState["alpm"])
    }

    width: 600
    height: 400
    padding: 7

    property var config: Database.config

    onDialogAccepted: {
        applySettings();
        close();
    }
    onDialogApplied: {
        applySettings();
    }

    onDialogRejected: {
        close()
    }

    id: item1
    Item  {
        anchors{
            top:parent.top
            left:parent.left
            right:parent.right
            bottom: buttonBox.top
        }

        Pane{
            padding: 3
            id:tabDrawer
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top:parent.top
            width: 130
            background: Rectangle{
                color:systemPalette.alternateBase
                border.color: systemPalette.mid
                border.width: 1
            }
            ListView{
                currentIndex: 0
                id:tabBar
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar{
                    visible: true
                }

                anchors.fill: parent
                model:ListModel{
                    ListElement{
                        itemText:qsTr("General")
                    }
                    ListElement{
                        itemText:qsTr("Official repositories")
                    }
                    ListElement{
                        itemText:qsTr("AUR")
                    }
                    ListElement{
                        itemText:qsTr("Cache")
                    }
                }
                delegate:
                    MenuItemDelegate{
                    backgroundColor: systemPalette.alternateBase
                    highlighted: tabBar.currentIndex==index
                    text: itemText
                    width: parent.width
                    onClicked: {
                        tabBar.currentIndex=index
                    }
                }
            }
        }

        StackLayout {
            id: stackLayout
            currentIndex: tabBar.currentIndex
            anchors.left: tabDrawer.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.bottomMargin: 0



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

                    PreferencesCheckBox {
                        settingName: "RemoveUnrequiredDeps"
                        checked: config.recurse
                        id: checkBox
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                    }

                    Label {
                        id: label1
                        text: qsTr("Check available disk space")
                    }

                    PreferencesCheckBox {
                        settingName: "CheckSpace"
                        checked: Database.checkspace
                        id: checkBox1
                        tristate: false
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        onClicked: {
                            settingsState["alpm"][settingName] = pref
                        }
                    }


                    Label {
                        id: labelParallelDownloads
                        height: spinBox.height
                        text: qsTr("Maximum parallel downloads:")
                        verticalAlignment: Text.AlignVCenter
                    }
                    SpinBox {
                        id: spinBoxParallelDownloads
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        value: config.maxParallelDownloads
                        onValueModified: {
                            settingsState["pamac"]["MaxParallelDownloads"]=value;
                        }
                    }

                    Label {
                        id: labelDowngrade
                        text: qsTr("Enable downgrade")
                    }

                    PreferencesCheckBox {
                        checked: config.enableDowngrade
                        id: checkBoxDowngrade

                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        settingName: "EnableDowngrade"
                    }
                    Label {
                        id: label2
                        text: qsTr("Check for updates")
                    }

                    PreferencesCheckBox {
                        checked: config.downloadUpdates
                        id: checkBox2
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        settingName: "DownloadUpdates"
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
                        value:config.refreshPeriod
                        onValueModified: {
                            settingsState["pamac"]["RefreshPeriod"]=value;

                        }
                    }


                    PreferencesCheckBox {
                        checked: config.downloadUpdates
                        id: checkBox3
                        x: 0
                        text: qsTr("Automatically download updates")
                        anchors.top: spinBox.bottom
                        anchors.topMargin: 6
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.fillWidth: true
                        settingName: "DownloadUpdates"
                    }

                    PreferencesCheckBox {
                        checked: config.noUpdateHideIcon
                        id: checkBox4
                        x: 0
                        text: qsTr("Hide tray icon when no update available")
                        anchors.top: checkBox3.bottom
                        anchors.topMargin: 6
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        settingName: "NoUpdateHideIcon"
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
                            color:systemPalette.base
                            border.color: systemPalette.mid
                            border.width: 1
                        }

                        ListView {
                            ScrollBar.vertical: ScrollBar{
                                visible: true
                            }
                            id: listView
                            boundsBehavior: Flickable.StopAtBounds
                            clip: true
                            anchors{
                                left: parent.left
                                right: parent.right
                                top: parent.top
                                bottom: ignorePkgsButtonBox.top
                            }

                            delegate: ItemDelegate{
                                onClicked: {
                                    listView.currentIndex=index;
                                }
                                highlighted: listView.currentIndex==index
                                width:parent.width
                                text: modelData
                            }

                            model:Database.getIgnorePkgs()
                            Connections{
                                target: transaction
                                onWriteAlpmConfigFinished:{
                                    listView.model=Database.getIgnorePkgs();
                                }
                            }
                        }
                        Pane{
                            padding: 0
                            background: Rectangle{
                                color:"transparent"
                            }

                            Dialog{
                                id:inputDialog
                                visible: false
                                TextArea{
                                    placeholderText: qsTr("Enter package name")
                                    anchors.fill: parent
                                    id:inputPackageText
                                }
                                onAccepted: {

                                    var list = Database.getIgnorePkgs();
                                    list.push(inputPackageText.text);

                                    var string = list.join(" ");

                                    settingsState["alpm"]["IgnorePkg"]=string;

                                }
                            }

                            id:ignorePkgsButtonBox
                            anchors{
                                left: parent.left
                                right: parent.right
                                bottom: parent.bottom
                            }
                            Row{
                                Button{
                                    icon.name: "list-add"
                                    onClicked: {
                                        inputDialog.open();
                                    }
                                }
                                Button{
                                    icon.name: "list-remove"
                                    onClicked: {
                                        var list = Database.getIgnorePkgs();
                                        list.splice(listView.currentIndex,1);

                                        var string = list.join(" ");

                                        settingsState["alpm"]["IgnorePkg"]=string;
                                    }
                                }
                            }
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

                    model: Database.getMirrorsCountries();

                    currentIndex: model.indexOf(Database.getMirrorsChoosenCountry());
                    id: comboBox
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    onCurrentTextChanged:{
                        button.highlighted=true;
                    }
                }

                Button {
                    id: button
                    x: 528
                    y: 58
                    text: qsTr("Refresh mirror list")
                    anchors.right: parent.right
                    onClicked: {
                        transaction.startGenerateMirrorsList(comboBox.currentText=="Worldwide"?"all":comboBox.currentText)
                    }
                }
            }

            Pane {
                background: Rectangle{
                    color: systemPalette.alternateBase
                }
                id: page1
                Label {
                    background: Rectangle{
                        color: "#ff9100"
                    }
                    padding:5
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    color: "white"
                    id: label6
                    text: qsTr("AUR is a community maintened repository so it presents potential risks and problems.\nAll AUR users should be familiar with the build process.")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                }


                Label {
                    id: label7
                    height: aurEnabledCheckBox.height
                    text: qsTr("Enable AUR support")
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.top: aurEnabledCheckBox.top
                    anchors.topMargin: 0
                }

                PreferencesCheckBox {
                    checked: config.enableAur
                    id: aurEnabledCheckBox
                    x: 465
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.top: label6.bottom
                    anchors.topMargin: 16
                    settingName: "EnableAUR"
                }

                PreferencesCheckBox {
                    enabled: aurEnabledCheckBox.enabled
                    checked: config.checkAurUpdates
                    id: checkBox6
                    text: qsTr("Check for updates from AUR")
                    anchors.top: label7.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    settingName: "CheckAURUpdates"
                }
                PreferencesCheckBox {
                    enabled: aurEnabledCheckBox.enabled
                    checked: config.checkAurVCSUpdates
                    id: checkBoxVCSUpdates
                    text: qsTr("Check for development packages updates")
                    anchors.top: checkBox6.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    settingName: "CheckAURVCSUpdates"
                }

                Label {
                    enabled: aurEnabledCheckBox.enabled
                    id: label8
                    height: aurBuildDirTextArea.height
                    text: qsTr("Build directory:")
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: aurBuildDirTextArea.top
                    anchors.topMargin: 0
                    anchors.left: checkBox6.left
                    anchors.leftMargin: 0
                }

                TextArea {

                    text: config.aurBuildDirectory

                    enabled: false
                    id: aurBuildDirTextArea
                    x: 500
                    anchors.left: label8.right
                    anchors.top: checkBoxVCSUpdates.bottom
                    anchors.topMargin: 6
                    anchors.right: chooseButton.left
                    Connections{
                        target: transaction
                        onWritePamacConfigFinished:{
                            aurBuildDirTextArea.text = config.aurBuildDirectory;
                        }
                    }
                }
                Button{
                    anchors.top: aurBuildDirTextArea.top
                    enabled: aurEnabledCheckBox.enabled
                    id:chooseButton
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    icon.name: "folder"
                    onClicked: {
                        aurBuildDirDialog.open();
                    }
                }
                //                FileDialog{
                //                    folder: encodeURIComponent(aurBuildDirTextArea.text)
                //                    id:aurBuildDirDialog
                //                    selectFolder:true
                //                    selectMultiple: false
                //                    selectExisting: true
                //                    title: qsTr("Please choose a build directory")
                //                    onAccepted: {
                //                        var path = Utils.urlToPath(aurBuildDirDialog.fileUrl.toString());

                //                        var obj = {"BuildDirectory":path};
                //                        transaction.startWritePamacConfig(obj);
                //                    }
                //                }
                RowLayout{
                    enabled: aurEnabledCheckBox.enabled
                    anchors.top: aurBuildDirTextArea.bottom
                    anchors.left: aurBuildDirTextArea.left
                    anchors.right: chooseButton.right
                    Button{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        id:defaultLocationButton
                        text:qsTr("Default")
                        onClicked: {
                            var path = "/var/tmp"

                            settingsState["pamac"]["BuildDirectory"]=path;
                        }
                    }
                    Button{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        id:tmpLocationButton
                        text:qsTr("Temporary directory")
                        onClicked: {
                            var path = Utils.urlToPath(StandardPaths.standardLocations(StandardPaths.TempLocation)[0]);

                            settingsState["pamac"]["BuildDirectory"]=path;
                        }
                    }
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
                    onValueModified: {
                        settingsState["pamac"]["KeepNumPackages"]=value
                    }
                    value: config.cleanKeepNumPkgs
                    id: spinBox1
                    x: 493
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.top: parent.top
                    anchors.topMargin: 6
                }

                PreferencesCheckBox {
                    checked: config.cleanRmOnlyUninstalled
                    id: checkBox7
                    text: qsTr("Remove only the versions of uninstalled packages")
                    anchors.top: label9.bottom
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    settingName: "OnlyRmUninstalled"
                }

                Button {
                    id: button1
                    x: 500
                    text: qsTr("Clean cache")
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.top: checkBox7.bottom
                    anchors.topMargin: 6
                    onClicked: {
                        transaction.cleanCache(config.cleanKeepNumPkgs,config.cleanRmOnlyUninstalled);
                    }
                }
            }
        }
    }
    DialogButtonBox{
        id:buttonBox
        anchors{
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Apply | DialogButtonBox.Cancel
        onAccepted: dialogAccepted()
        onRejected: dialogRejected()
        onApplied: dialogApplied()
    }
}
