import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0
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

    function search()
    {

        if(tempModel===undefined){
            drawer.push("SideMenuSearch.qml")
            tempModel = mainView.packageList;
        }

        if(text.length>0){
            mainView.title = qsTr("Search Results for ")+searchPane.text;
            if(drawer.currentItem.currentIndex===Database.Repos){
                mainView.packageListFuture=Database.searchPkgsAsync(text);
            } else if(drawer.currentItem.currentIndex===Database.AUR){
                mainView.packageListFuture=Database.searchPkgsInAurAsync(text);
            }
        }
        else {
            mainView.title = "";
            drawer.pop();
            mainView.packageList = tempModel;
            tempModel = undefined;
        }


    }
    Connections{
        target: drawer.currentItem
        onCurrentIndexChanged: {
            if(text.length>0){
                if(drawer.currentItem.currentIndex===Database.Repos){
                    mainView.packageListFuture=Database.searchPkgsAsync(text);
                } else if(drawer.currentItem.currentIndex===Database.AUR){
                    mainView.packageListFuture=Database.searchPkgsInAurAsync(text);
                }
            }
        }
    }

    TextField{
        validator: RegExpValidator{
            regExp: /^[^ ][\w\W ]*[^ ]/
        }

        placeholderText: qsTr("Search...")
        focus: true
        id:searchArea
        width: parent.width
        leftPadding: searchImage.width + 6
        anchors.centerIn: parent

        onTextChanged: {
            search();
        }
    }
    property var tempModel
}
