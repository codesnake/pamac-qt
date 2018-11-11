import QtQuick 2.4
import QtQuick.Dialogs 1.2
import Pamac.alpm.database 1.0
Dialog{

    title: "Preferences"
    width: 600
    height: 400

    standardButtons: Dialog.Close
    PreferencesDialogForm {
        id:form
        config: Database.config
    }
}
