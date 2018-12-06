import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import Pamac.Config 1.0
import Pamac.Database 1.0
import Pamac.PackageModel 1.0

CheckBox {
    property var settingName
    id: checkBox
    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
    onCheckedChanged: {
        transaction.startWritePamacConfig({settingName:checked});
    }
}
