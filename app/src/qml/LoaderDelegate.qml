import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.PackageModel 1.0
import QPamac.Database 1.0

Rectangle{

    implicitHeight: 45

    id:delegate

    signal clicked
    signal doubleClicked
    property var modelData: model

    property list<Component> columns
    function columnGettingFunction(column){
        return columns[column]
    }

    property bool highlighted:false
    property bool hovered
    color: (hovered || itemMouseArea.containsMouse)?systemPalette.midlight:!highlighted?systemPalette.base:systemPalette.highlight
    readonly property  alias containsMouse:itemMouseArea.containsMouse
    MouseArea{
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        id:itemMouseArea
        hoverEnabled: true
        onClicked: {
           delegate.clicked(mouse)
        }
        onDoubleClicked: {
          delegate.doubleClicked(mouse);
        }

        anchors.fill: parent
    }

    Loader{
        property var modelData:delegate.modelData
        id:cellLoader
        anchors.fill: parent
        anchors.leftMargin: 2

        Component.onCompleted: {
            sourceComponent=columnGettingFunction(column)

        }
    }
    TableView.onReused: {
        cellLoader.sourceComponent=columnGettingFunction(column)
    }
}
