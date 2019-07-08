#include <Config.h>
#include <QApplication>
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

    LibQPamacModule module;
    module.registerTypes(nullptr);

    QQuickStyle::setFallbackStyle("fusion");
    QQuickStyle::setStyle("org.kde.desktop");

    qmlRegisterSingletonType<NotificationService>("NotificationService",1,0,"NotificationService",
                                                  [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(scriptEngine)
        auto qmlApplicationEngine = qobject_cast<QQmlApplicationEngine*>(engine);
        auto showAction = new QAction("Show");
        auto exitAction = new QAction("Exit");
        auto updatesAction = new QAction("Show Updates");
        auto icon = new QSystemTrayIcon(engine);



        QObject::connect(updatesAction,&QAction::triggered,engine,[=](){
            showAction->trigger();
            QMetaObject::invokeMethod(qmlApplicationEngine->rootObjects()[0],"showUpdates");
        });
        QObject::connect(showAction,&QAction::triggered,engine,[=](){QMetaObject::invokeMethod(qmlApplicationEngine->rootObjects()[0],"show");});
        QObject::connect(exitAction,&QAction::triggered,engine,[=](){engine->quit();});
        QObject::connect(icon,&QSystemTrayIcon::activated,showAction,&QAction::triggered,Qt::DirectConnection);

        auto menu = new QMenu("Main");
        menu->addActions({
                             showAction,
                             updatesAction,
                             exitAction
                         });
        icon->setIcon(QIcon::fromTheme("system-software-install"));
        icon->setContextMenu(menu);
        icon->setToolTip("Package management tool");

        return NotificationService::NotificationServiceBuilder()
                .probeDBus()
                ->addTrayIcon(icon)->build(engine);
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
