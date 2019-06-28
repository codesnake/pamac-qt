import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QPamac.PackageModel 1.0

Page{
    property var summary
    width: 600
    height: 400
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
            SummaryCategoryListView {
                visible: summary.toInstall.length>0
                text: qsTr("To Install:")
                packageList: summary.toInstall

            }
            SummaryCategoryListView {
                visible: summary.toRemove.length>0
                text: qsTr("To Remove:")
                packageList: summary.toRemove

            }
            SummaryCategoryListView {
                visible: summary.toReinstall.length>0
                text: qsTr("To Reinstall:")
                packageList: summary.toReinstall

            }
            SummaryCategoryListView {
                visible: summary.toUpgrade.length>0
                text: qsTr("To Upgrade:")
                packageList: summary.toUpgrade
            }
            SummaryCategoryListView {
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
    signal dialogRejected
}
