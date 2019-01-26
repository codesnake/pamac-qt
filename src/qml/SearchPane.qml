import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0
Pane{
    function append(text){
        searchArea.append(text);
    }

    property alias text: searchArea.text
    property alias searchFocus:searchArea.focus
    states:[
        State{
            name:"opened"
            PropertyChanges {
                target: searchPane
                height:searchArea.height+10
            }
        },
        State{
            name:"hidden"
            PropertyChanges {
                target: searchPane
                height:0
                text:""
            }

        }
    ]
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

    
    TextArea{

        Image{
            anchors.verticalCenter: parent.verticalCenter
            id:searchImage
            source: "image://icons/search"
            anchors.left: parent.left
            height: parent.height*0.75
            width: height
            sourceSize.width: width
            sourceSize.height: height
        }

        focus: true
        id:searchArea
        width: parent.width/2
        leftPadding: searchImage.width + 6
        anchors.centerIn: parent

        onTextChanged: {


            if(tempModel===undefined){
                drawer.push("SideMenuSearch.qml")
                tempModel = mainView.packageList;
            }

            if(text.length>0){
                mainView.packageListFuture=Database.searchPkgsAsync(text);
            }
            else {
                drawer.pop();
                mainView.packageList = tempModel;
                tempModel = undefined;
            }


        }
    }
    property var tempModel
}
