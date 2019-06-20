import QtQuick 2.12
import QtQuick.Controls 2.4
import Pamac.PackageModel 1.0
import Pamac.Database 1.0

Rectangle{

    implicitHeight: 45

    id:delegate

    signal onClicked
    signal onDoubleClicked
    property var modelData: model

    property list<Component> columns

    property bool highlighted:isSelected(row)
    color: highlighted?systemPalette.highlight:systemPalette.base

    MouseArea{
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        id:itemMouseArea
        onClicked: {
           delegate.onClicked(mouse)
        }
        onDoubleClicked: {
          delegate.onDoubleClicked(mouse);
        }

        anchors.fill: parent
    }

    Loader{
        property var modelData:delegate.modelData
        id:cellLoader
        anchors.fill: parent
        anchors.leftMargin: 2

        Component.onCompleted: {
            sourceComponent=columns[column]

        }
    }
    TableView.onReused: {
        cellLoader.sourceComponent = columns[column]
    }
}
