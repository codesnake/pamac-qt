import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamaq.alpm.database 1.0
import Pamaq.alpm.package 1.0

ItemDelegate{
    onDoubleClicked: {
        stackView.push("PagePackageInfoForm.ui.qml",{"pkg":Database.getPkgDetails(name,appName)})
    }

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter
        Item {
            Layout.preferredWidth: listView.headerItem.itemAt(0).width
            Rectangle{
                color:installedVersion!=""?"lightgreen":"red"
                width:10
                height: 10
                anchors.centerIn: parent
            }
        }
        Row{
            height: parent.height
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: listView.headerItem.itemAt(1).width
            spacing: 5
            Image{
                anchors.verticalCenter: parent.verticalCenter
                id:packageIcon;
                width: 25
                height: width
                source:iconUrl.toString().length?Qt.resolvedUrl("file://"+iconUrl):"file:///usr/share/icons/breeze/mimetypes/64/package-x-generic.svg"
            }
            
            Column{
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-packageIcon.width-5
                Label {
                    width:parent.width
                    font.bold: true
                    text:appName?appName:name
                    elide: Text.ElideRight
                }
                Label {
                    width:parent.width
                    text:desc
                    elide: Text.ElideRight
                }
            }
        }
        Label{
            text:version
            Layout.preferredWidth: listView.headerItem.itemAt(2).width
        }
        Label{
            text:repo
            Layout.preferredWidth: listView.headerItem.itemAt(3).width
        }
        Label{
            text:size.toString()
            Layout.preferredWidth: listView.headerItem.itemAt(4).width
        }
        width: parent.width
    }
}
