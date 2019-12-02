import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
import QPamac.Async 1.0
import Qt.labs.platform 1.0 as Labs
import DialogRunner 1.0
import "./" as PamacQt
import "../../js/JSUtils.js" as JSUtils

ToolButton {
    checkable: true;
    id: toolButton1
    flat: true
    font.pixelSize: Qt.application.font.pixelSize * 1.4
    Menu {
        onClosed: toolButton1.checked=false
        
        visible: toolButton1.checked
        y: toolButton1.height
        id: contextMenu
        Action {
            icon.name: "view-refresh-symbolic"
            text: "Refresh databases"
            onTriggered: {
                transaction.startSysupgrade(true,false,[],[]);
                
            }
        }
        Action {
            icon.name: "edit-undo-history"
            text: "View History"
            onTriggered: {
                DialogRunner.exec(Qt.resolvedUrl("../Dialogs/HistoryDialog.qml"));
            }
        }
        Labs.FileDialog{
            id: fileDialog
            fileMode: Labs.FileDialog.OpenFiles
            nameFilters: ["ALPM packages (*.pkg.tar.*)"]
            onAccepted: {
                transaction.start([],[],fileDialog.files.map(value=>value.toString().replace("file://","")))
            }
        }
        
        Action {
            icon.name: "package-x-generic"
            text: "Install local packages"
            onTriggered: {
                fileDialog.open()
            }
        }
        Action {
            icon.name: "settings-configure"
            text: "Preferences"
            onTriggered: {
                DialogRunner.exec(Qt.resolvedUrl("../Dialogs/PreferencesDialog.qml"),{"transaction":transaction})
            }
        }
        Action {
            icon.name: "help-about-symbolic"
            text: "About"
            onTriggered: {
                DialogRunner.exec(Qt.resolvedUrl("../Dialogs/AboutDialog.qml"));
            }
        }
    }
}
