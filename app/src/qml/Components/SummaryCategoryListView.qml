import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QPamac.PackageModel 1.0

ListView{
    id:listView
    property var packageList
    property string text

    header: Label{
        text:listView.text
        id: headerLabel
        font.weight: Font.Bold
    }

    height: childrenRect.height
    width: parent.width
    interactive:false
    model: packageList

    delegate: RowLayout{
        width: parent.width
        Label{
            text:modelData.name
            Layout.fillWidth: true
        }
        Label{
            text:modelData.version
        }
        Label{
            text:modelData.repo
        }
        Label{
            text:downloadSize?downloadSize.toString():0
            visible: modelData.downloadSize>0
        }
    }
}
