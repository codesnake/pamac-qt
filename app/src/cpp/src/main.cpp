#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickStyle>
#include <QtQuickControls2>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <libqpamac>
#include <NotificationService.h>
#include <XDGIconProvider.h>
#include <QmlDialogRunner.h>
#include <QQmlDebuggingEnabler>
static QQmlDebuggingEnabler enabler;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("Artem Grinev");
    QGuiApplication::setApplicationName("Pamac-Qt");
    //    QGuiApplication::setApplicationVersion(QString(VERSION));
    QGuiApplication::setWindowIcon(QIcon::fromTheme("system-software-install"));
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.setApplicationDescription("Pamac Qt - a Qt5 graphical front end to libpamac");

    QCommandLineOption updatesOption({"u", "updates"},
                                     QCoreApplication::translate("main", "Open \"Updates\" page"));
    QCommandLineOption installOption({"i", "install"},
                                     QCoreApplication::translate("main", "Install a local <package>."),"package");
    parser.addOption(installOption);
    parser.addOption(updatesOption);

    parser.process(app);
    const QString& installFileName = parser.value(installOption);

    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

    LibQPamacModule module;
    module.registerTypes(nullptr);

    QQuickStyle::setFallbackStyle("fusion");
    QQuickStyle::setStyle("org.kde.desktop");

    qmlRegisterSingletonType<NotificationService>("NotificationService",1,0,"NotificationService",
                                                  [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(scriptEngine)


        return NotificationService::createDefault(engine);
    });
    qmlRegisterSingletonType<QmlDialogRunner>("DialogRunner",1,0,"DialogRunner",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine)->QObject*{
        Q_UNUSED(scriptEngine)

        return new QmlDialogRunner(engine);
    });

    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("icons"), new XDGIconProvider);
    if(QFileInfo::exists(installFileName)){
        engine.load(QUrl(QStringLiteral("qrc:/src/qml/OpenWithDialog.qml")));
    } else{
        engine.load(QUrl(QStringLiteral("qrc:/src/qml/MainWindow.qml")));
        if(parser.isSet(updatesOption)){
            QMetaObject::invokeMethod(engine.rootObjects().at(0),"showUpdates");
        }
    }

    if (engine.rootObjects().isEmpty()){
        return -1;
    }

    return QGuiApplication::exec();

}
