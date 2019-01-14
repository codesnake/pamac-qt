import QtQuick 2.9
import QtQuick.Controls 2.2
import Pamac.Database 1.0

ItemDelegate {
    property color backgroundColor: systemPalette.window
    background: Rectangle{
        Behavior on color{

            ColorAnimation {
                duration: 200
            }
        }

        color: {
            if(highlighted){
                if(hovered){
                    return Qt.darker(systemPalette.highlight,1.1);
                }
                return systemPalette.highlight;
            }

            if(down){
                return systemPalette.highlight;
            }

            if(hovered){
                return Qt.lighter(systemPalette.highlight,1.99)
            }

            return backgroundColor;
        }

    }
    highlighted: index==currentIndex
    width: parent.width
    height:45
    leftPadding: 15
    font.pixelSize: 12
}
