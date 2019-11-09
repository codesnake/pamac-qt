import QtQuick 2.13
import QtQuick.Controls 2.13
import "../../js/JSUtils.js"

Item {
    Row{
        anchors.left: parent.left
        anchors.leftMargin: 20
        height: parent.height
        spacing: 5
        Image{
            anchors.verticalCenter: parent.verticalCenter
            id:packageIcon
            width: 45
            height: width
            source:(JSUtils.isAccessible(iconUrl) && iconUrl.toString().length)?Qt.resolvedUrl("file://"+iconUrl):"image://icons/package-x-generic"
        }

        Column{
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width-packageIcon.width-5
            Label {

                width:parent.width
                text:appName?appName+" ("+name+")":name
                font.weight: Font.Bold
                font.bold: true

                elide: Text.ElideRight


            }
            Label {
                width:parent.width
                text:desc
                elide: Text.ElideRight
            }
            Label{
                clip: true
                text:repo
            }
        }

    }
}
