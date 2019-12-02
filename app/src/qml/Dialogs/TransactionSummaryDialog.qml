import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.PackageModel 1.0

import "../Components" as Components

Page{
    property var summary
    property var result
    anchors.fill: parent
    width: 600
    height: 400
    id:dialog
    Flickable{
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar{
            active:true
        }
        anchors{
            margins: 6
            top:parent.top
            bottom: buttonBox.top
            left: parent.left
            right: parent.right
        }
        contentHeight: column.height
        Column{
            id:column
            height: childrenRect.height
            width: parent.width
            Components.SummaryCategoryListView {
                visible: summary.toInstall.length>0
                text: qsTr("To Install:")
                packageList: summary.toInstall

            }
            Components.SummaryCategoryListView {
                visible: summary.toRemove.length>0
                text: qsTr("To Remove:")
                packageList: summary.toRemove
            }
            Components.SummaryCategoryListView {
                visible: summary.toReinstall.length>0
                text: qsTr("To Reinstall:")
                packageList: summary.toReinstall

            }
            Components.SummaryCategoryListView {
                visible: summary.toUpgrade.length>0
                text: qsTr("To Upgrade:")
                packageList: summary.toUpgrade
            }
            Components.SummaryCategoryListView {
                visible: summary.toBuild.length>0
                text: qsTr("To Build:")
                packageList: summary.toBuild


            }
        }
    }
    DialogButtonBox{
        id:buttonBox
        anchors{
            margins: 6
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
        onAccepted: dialogAccepted()
        onRejected: dialogRejected()
    }
    signal dialogAccepted
    onDialogAccepted: {
        result = true;
        close();
    }
    onDialogRejected: {
        result = false;
        close();
    }
    signal dialogRejected
    signal close
}
