#include <Config.h>
#include <QApplication>
#include <QQmlContext>
#include <QQuickStyle>
#include <QtQuickControls2>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <libqpamac>
#include <XDGIconProvider.h>
#include <QmlDialogRunner.h>
#include <QQmlDebuggingEnabler>
#include <NotificationServiceModule.h>
static QQmlDebuggingEnabler enabler;

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QApplication::setOrganizationName("LordTermor");
    QApplication::setApplicationName("Pamac-Qt");
    QApplication::setApplicationVersion(QString(VERSION));
    QApplication::setWindowIcon(QIcon::fromTheme("system-software-install"));
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

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

    LibQPamacModule qPamacModule;
    qPamacModule.registerTypes(nullptr);

    NotificationServiceModule notificationServiceModule("Pamac Qt");
    notificationServiceModule.registerTypes(nullptr);

    QQuickStyle::setFallbackStyle("fusion");
    QQuickStyle::setStyle("org.kde.desktop");


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

    QObject::connect(&engine,&QQmlApplicationEngine::quit,&engine,[&](){
        QSettings settings;
        auto list = engine.rootObjects();
        settings.beginGroup("Window");
        for(const QByteArray& el : {"x","y","width","height"}){
            settings.setValue(el,list[0]->property(el));
        }
        settings.endGroup();
        settings.sync();
    });


    if (engine.rootObjects().isEmpty()){
        return -1;
    }
    QSettings settings;
    auto list = engine.rootObjects();
    settings.beginGroup("Window");
    for(const QByteArray& el : {"x","y","width","height"}){
        QVariant setting;
        if((setting = settings.value(el,QVariant::Invalid))!=QVariant::Invalid){
            list[0]->setProperty(el,settings.value(el));
        }
    }
    return QApplication::exec();

}
