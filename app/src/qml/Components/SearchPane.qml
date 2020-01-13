import QtQuick 2.9
import QtQuick.Controls 2.12
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
        if(/^((?=[^\x00-\x1F\x7F])\S)$/.test(event.text)){
            text = event.text;
            searchArea.forceActiveFocus();
        }

    }

    function search()
    {
        mainView.title = qsTr("Search Results for ")+searchPane.text;
        switch(drawer.currentItem.currentIndex){
        case Database.Repos:
            mainView.packageList=Database.searchPkgs(text);
            break;
        case Database.AUR:
            mainView.packageList=Database.searchPkgsInAur(text);
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
            regExp: /^((?!(?:\$.)|(?:\s{2}))[^\s].)*$/
        }

        placeholderText: qsTr("Search...")
        focus: true
        id:searchArea
        width: parent.width
        anchors.centerIn: parent

        onTextChanged: {
            if(tempModel===undefined){
                drawer.push("SideMenu/SideMenuSearch.qml")
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
