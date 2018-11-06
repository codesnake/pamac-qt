import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Pamac.alpm.database 1.0
import Pamac.alpm.package 1.0

ItemDelegate{
    highlighted: index==currentIndex
    onClicked: {
        currentIndex=index
    }

    onDoubleClicked: {
        stackView.push("PagePackageInfoForm.ui.qml",{"pkg":Database.getPkgDetails(name,appName)})
    }

    RowLayout {
        spacing: 3
        clip: true
        anchors.verticalCenter: parent.verticalCenter
        Item {
            Layout.preferredWidth: listView.headerItem.itemAt(0).width
            Rectangle{
                color:installedVersion!=""?"lightgreen":"red"
                width:15
                height: 15
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
                id:packageIcon
                width: 25
                height: width
                source:iconUrl.toString().length?Qt.resolvedUrl("file://"+iconUrl):"file:///usr/share/icons/breeze/mimetypes/64/package-x-generic.svg"
            }

            Column{
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-packageIcon.width-5
                Label {

                    width:parent.width
                    text:appName?appName+" ("+name+")":name
                    font.bold: true
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
            clip: true
            text:version
            Layout.preferredWidth: listView.headerItem.itemAt(2).width
        }
        Label{
            clip: true
            text:repo
            Layout.preferredWidth: listView.headerItem.itemAt(3).width
        }
        Label{
            clip: true
            text:size.toString()
            Layout.preferredWidth: listView.headerItem.itemAt(4).width
        }
    }
}
