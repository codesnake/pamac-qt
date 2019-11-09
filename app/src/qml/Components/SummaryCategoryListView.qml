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
    model: PackageModel{
        id:packageModel
        packageList: listView.packageList
    }

    delegate:Label{
        width: parent.width
        height: childrenRect.height
        RowLayout{
            width: parent.width
            Label{
                text:name
                Layout.fillWidth: true
            }
            Label{
                text:version
            }
            Label{
                text:repo
            }
            Label{
                text:downloadSize?downloadSize.toString():0
                visible: downloadSize>0
            }
            
        }
    }
}
