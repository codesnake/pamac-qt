QT += quick gui widgets quickwidgets quickcontrols2
CONFIG += c++1z
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS+=/usr/lib/libpamac.so /usr/lib/libgobject-2.0.so /usr/lib/libglib-2.0.so
INCLUDEPATH += "/usr/lib/glib-2.0/include" /usr/include/glib-2.0/ ./src/cpp/include/

QMAKE_CXXFLAGS += -std=c++17

HEADERS += \
    src/cpp/include/Config.h \
    src/cpp/include/Database.h \
    src/cpp/include/Package.h \
    src/cpp/include/PackageList.h \
    src/cpp/include/PackageModel.h \
    src/cpp/include/QQuickDialog.h \
    src/cpp/include/Transaction.h \
    src/cpp/include/Updates.h \
    src/cpp/include/Utils.h

SOURCES += \
    src/cpp/src/Config.cpp \
    src/cpp/src/Database.cpp \
    src/cpp/src/main.cpp \
    src/cpp/src/PackageModel.cpp \
    src/cpp/src/QQuickDialog.cpp \
    src/cpp/src/Transaction.cpp \
    src/cpp/src/Utils.cpp
