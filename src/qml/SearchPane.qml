import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Pamac.Database 1.0
import Pamac.PackageModel 1.0
import Pamac.Transaction 1.0
Pane{
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
    anchors{
        top:parent.top
        right:parent.right
        left:mainView.left
    }
    
    TextArea{
       Image{

            id:searchImage
            source: "image://icons/search"
            anchors.left: parent.left
            height: parent.height
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
                tempModel = mainView.modelData;
            }

            if(text.length>0){
                mainView.modelData=Database.searchPkgs(text);
            }
            else {
                mainView.modelData = tempModel;
                tempModel = undefined;
            }


        }
    }
    property var tempModel
}
