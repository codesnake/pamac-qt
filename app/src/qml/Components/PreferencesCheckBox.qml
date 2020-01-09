import QtQuick 2.4
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0

CheckBox {
    property var settingName
    id: checkBox
    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
    onClicked: {
        settingsState["pamac"][settingName]=checked;
    }
}
