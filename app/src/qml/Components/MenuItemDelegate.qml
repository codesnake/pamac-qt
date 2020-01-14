import QtQuick 2.12
import QtQuick.Controls 2.12
import QPamac.Database 1.0

ItemDelegate {
    SystemPalette{
        id:systemPalette
    }

    property color backgroundColor: Qt.rgba(systemPalette.window.r,
                                            systemPalette.window.g,
                                            systemPalette.window.b,
                                            0)
    background: Rectangle{
        Behavior on color{

            ColorAnimation {
                duration: 200
            }
        }

        color: {
            if(highlighted){
                return systemPalette.highlight;
            }

            if(down){
                return systemPalette.dark;
            }

            if(hovered){
                return Qt.tint(systemPalette.highlight,systemPalette.window)
            }

            return backgroundColor;
        }

    }
    highlighted: typeof(index) !== "undefined" && index===currentIndex
    width: parent.width
    height:30
    leftPadding: 15
    font.pointSize: 9
}
