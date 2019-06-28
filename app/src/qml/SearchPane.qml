import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QPamac.Database 1.0
import QPamac.PackageModel 1.0
import QPamac.Transaction 1.0
Pane{
    padding:0
    function append(text){
        searchArea.append(text);
    }

    property alias text: searchArea.text
    property alias searchFocus:searchArea.focus

    transitions: [
        Transition {
            from: "*"
            to:"*"
            animations:[
                NumberAnimation{
                    properties: "height"
                }
            ]
        }
    ]
    id:searchPane

    Keys.onPressed: {
        if(/^[^\s][\w\S]*$/.test(event.text) &&
                (event.modifiers===Qt.NoModifier ||
                 event.modifiers===Qt.ShiftModifier)
                && event.key!==Qt.Key_Backspace
                && event.key!==Qt.Key_Delete){
            text = event.text;
            searchArea.forceActiveFocus();
        }

    }

    function search()
    {
        mainView.title = qsTr("Search Results for ")+searchPane.text;
        switch(drawer.currentItem.currentIndex){
        case Database.Repos:
            mainView.packageListFuture=Database.searchPkgsAsync(text);
            break;
        case Database.AUR:
            mainView.packageListFuture=Database.searchPkgsInAurAsync(text);
            break;
        }



    }
    Connections{
        target: drawer.currentItem
        onCurrentIndexChanged: {
            if(text.length>0){
                search();
            }
        }
    }


    TextField{
        property Timer timer: Timer{
            interval: 700
            onTriggered: search()
        }

        validator: RegExpValidator{
            id:validator
            regExp: /^[^\s][\w\S]+$/
        }

        placeholderText: qsTr("Search...")
        focus: true
        id:searchArea
        width: parent.width
        leftPadding: searchImage.width + 6
        anchors.centerIn: parent

        onTextChanged: {
            if(tempModel===undefined){
                drawer.push("SideMenuSearch.qml")
                tempModel = mainView.packageList;
            }
            if(text.length!=0){
                if(timer.running){
                    timer.restart();
                } else{
                    timer.start();
                }
            }
            else {
                timer.stop();
                mainView.title = "";
                drawer.pop();
                mainView.packageList = tempModel;
                tempModel = undefined;
            }

        }
    }
    property var tempModel
}
