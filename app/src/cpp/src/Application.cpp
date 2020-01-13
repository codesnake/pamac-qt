#include "Application.h"
#include <QApplication>
#include <QIcon>
#include <QQuickWindow>
#include <libqpamac>
#include <QQuickStyle>
#include <QmlDialogRunner.h>
#include <NotificationServiceModule.h>
#include <XDGIconProvider.h>
#include <Utils.h>
#include <QFileInfo>
#include <QSettings>
#include <QtDBus>
#include <QtDebug>
QMap<QString,QObject*> Application::m_objects;
Application::Application(QObject *parent):QObject(parent)
{}

int Application::exec()
{
    registerModules();
    registerCommandLineOptions();

    auto fileinfo = QFileInfo(m_parser.value("install"));

    if(!registerDbusService() && !fileinfo.exists()){
        return 0;
    }

    connect(&m_engine,&QQmlApplicationEngine::objectCreated,this,[](QObject* element,const QUrl& url){
        if(element->objectName().isEmpty()){
            m_objects[url.toString()] = element;
        } else {
            m_objects[element->objectName()] = element;

        }
    });


    QApplication::setOrganizationName("manjaro");
    QApplication::setApplicationName("pamac-qt");
    QApplication::setApplicationVersion(QString(VERSION));
    QApplication::setWindowIcon(QIcon::fromTheme("system-software-install"));
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

    QQuickStyle::setFallbackStyle("fusion");
    QQuickStyle::setStyle("org.kde.desktop");




    if(fileinfo.exists()){
        loadOpenWithDialog(fileinfo.absoluteFilePath());
    } else {
        loadTrayIcon();
        if(!m_parser.isSet("tray-only")){
            loadMainWindow();
            if(m_parser.isSet("updates")){
                QMetaObject::invokeMethod(m_objects["mainWindow"],"showUpdates");
            }
        }
    }

    if (m_engine.rootObjects().isEmpty()){
        return -1;
    }
    return QApplication::exec();
}

void Application::saveGeometry()
{
    QSettings settings;

    settings.beginGroup("Window");
    for(const QByteArray& el : {"x","y","width","height"}){
        settings.setValue(el,m_objects["mainWindow"]->property(el));
    }
    settings.endGroup();
    settings.sync();
}

void Application::restoreGeometry()
{
    QSettings settings;
    settings.beginGroup("Window");
    for(const QByteArray& el : {"x","y","width","height"}){
        QVariant setting;
        if((setting = settings.value(el,QVariant::Invalid))!=QVariant::Invalid){
            m_objects["mainWindow"]->setProperty(el,settings.value(el));
        }
    }

}

void Application::loadMainWindow()
{
    qmlRegisterSingletonType<QmlDialogRunner>("DialogRunner",1,0,"DialogRunner",
                                              [](QQmlEngine *m_engine, QJSEngine *scriptEngine)->QObject*{
        Q_UNUSED(scriptEngine)

        return new QmlDialogRunner("mainWindow", m_engine);
    });

    if(m_objects["mainWindow"]!=nullptr){
        return;
    }

    m_engine.load(QUrl(QStringLiteral("qrc:/src/qml/Pages/MainWindow.qml")));
    restoreGeometry();

    Utils::LambdaHelper::connect(m_objects["mainWindow"],SIGNAL(sClosing()),[&](){
        saveGeometry();
        m_objects.take("mainWindow")->deleteLater();
    });

}

void Application::loadOpenWithDialog(const QString &filename)
{
    qmlRegisterSingletonType<QmlDialogRunner>("DialogRunner",1,0,"DialogRunner",
                                              [](QQmlEngine *m_engine, QJSEngine *scriptEngine)->QObject*{
        Q_UNUSED(scriptEngine)

        return new QmlDialogRunner("openWithDialog", m_engine);
    });

    m_engine.load(QUrl(QStringLiteral("qrc:/src/qml/Dialogs/OpenWithDialog.qml")));
    QMetaObject::invokeMethod(m_objects["openWithDialog"],"start",Q_ARG(QVariant,filename));
}

void Application::loadTrayIcon()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/src/qml/Components/TrayIcon.qml")));
    Utils::LambdaHelper::connect(m_objects["trayIcon"],SIGNAL(showUpdates()),[&](){
        QMetaObject::invokeMethod(m_objects["trayIcon"],"showApp");
        QMetaObject::invokeMethod(m_objects["mainWindow"],"showUpdates");
    });
    connect(m_objects["trayIcon"],SIGNAL(showApp()),this,SLOT(loadMainWindow()));
    Utils::LambdaHelper::connect(m_objects["trayIcon"],SIGNAL(quitApp()),[=](){
        if(m_objects["mainWindow"]!=nullptr){
            saveGeometry();
        }
        QApplication::quit();
    });
}

void Application::registerCommandLineOptions()
{
    m_parser.addHelpOption();
    m_parser.setApplicationDescription("Pamac Qt - a Qt5 graphical frontend for libpamac");

    QCommandLineOption updatesOption({"u", "updates"},
                                     QCoreApplication::translate("main", "Open \"Updates\" page"));
    QCommandLineOption installOption({"i", "install"},
                                     QCoreApplication::translate("main", "Install a local <package>."),"package");
    QCommandLineOption trayOption("tray-only",
                                  QCoreApplication::translate("main", "Open minimized to tray"));
    m_parser.addOption(installOption);
    m_parser.addOption(updatesOption);
    m_parser.addOption(trayOption);

    m_parser.process(*QApplication::instance());
}

void Application::registerModules()
{

    LibQPamacModule qPamacModule;
    qPamacModule.registerTypes(nullptr);

    NotificationServiceModule notificationServiceModule("Pamac Qt");
    notificationServiceModule.registerTypes(nullptr);


    m_engine.addImageProvider(QLatin1String("icons"), new XDGIconProvider);


}

bool Application::registerDbusService()
{
    QDBusInterface interface("LordTermor.PamacQt", "/", "", QDBusConnection::sessionBus());
    if(interface.isValid()){
        interface.call("loadMainWindow");
        return false;
    }
    QDBusConnection::sessionBus().registerService("LordTermor.PamacQt");
    QDBusConnection::sessionBus().registerObject("/",this,QDBusConnection::ExportAllSlots);
    return true;

}
