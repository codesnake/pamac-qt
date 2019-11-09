import QtQuick 2.12
import QtQuick.Controls 2.12
Image {
    property var minimumHeight: 400
    property var minimumWidth: minimumHeight * (sourceSize.width / sourceSize.height)
    fillMode: Image.PreserveAspectFit
    property string title: "Screenshot"
    anchors.fill: parent
}
