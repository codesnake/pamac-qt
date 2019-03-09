#include "pamac.h"
#include <Utils.h>
#include <QQmlContext>
#include <QApplication>
#include <QQuickStyle>
#include <QtQuickControls2>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <Database.h>
#include <QIcon>
#include <PackageModel.h>
#include <AURPackageModel.h>
#include <Updates.h>
#include <Transaction.h>
#include <XDGIconProvider.h>
#include "AsyncHelpers.h"
#ifdef QT_DEBUG
#include <QQmlDebuggingEnabler>
static QQmlDebuggingEnabler enabler;
#endif


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QApplication::setOrganizationName("Artem Grinev");
    QApplication::setApplicationName("PamacQt");
    QApplication::setWindowIcon(QIcon::fromTheme("package-x-generic"));
    QIcon::fromTheme("go-previous");

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
    QString installFileName = parser.value(installOption);




    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

    qRegisterMetaType<PamacQt::RepoPackageList>("RepoPackageList");
    qRegisterMetaType<PamacQt::AURPackageList>("AURPackageList");
    qRegisterMetaType<PamacQt::AURPackage>("AURPackage");
    qRegisterMetaType<PamacQt::RepoPackageDetails>("RepoPackageDetails");
    qRegisterMetaType<PamacQt::RepoPackage>("RepoPackage");
    qRegisterMetaType<PamacQt::Updates>("Updates");
    qRegisterMetaType<PamacQt::Config>("Config");
    qRegisterMetaType<PamacQt::TransactionSummary>("TransactionSummary");
    qRegisterMetaType<QmlFuture>("Future");

    //A (dirty) work around the bug that causes icons not to load in kde: manually set icon theme name
    if(QIcon::themeName().isEmpty()){
        QIcon::setThemeName("breeze");
    }

    QQuickStyle::setFallbackStyle("fusion");
    QQuickStyle::setStyle("org.kde.desktop");

    qmlRegisterSingletonType<PamacQt::Database>("Pamac.Database",1,0,"Database",
                                                [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(scriptEngine)

        PamacQt::Database *database = new PamacQt::Database("/etc/pamac.conf",scriptEngine);
        return database;
    });

    qmlRegisterUncreatableType<QmlFuture>("Pamac.Async",1,0,"Future","");
    qmlRegisterType<QmlFutureWatcher>("Pamac.Async",1,0,"FutureWatcher");

    qmlRegisterUncreatableType<PamacQt::AURPackageList>("Pamac.AUR.Package",1,0,"AURPackageList","");
    qmlRegisterUncreatableType<PamacQt::AURPackage>("Pamac.AUR.Package",1,0,"AURPackage","");
    qmlRegisterUncreatableType<PamacQt::RepoPackageDetails>("Pamac.Package",1,0,"RepoPackageDetails","");
    qmlRegisterUncreatableType<PamacQt::RepoPackage>("Pamac.Package",1,0,"RepoPackage","");
    qmlRegisterUncreatableType<PamacQt::RepoPackageList>("Pamac.Package",1,0,"RepoPackageList","");
    qmlRegisterType<PamacQt::PackageModel>("Pamac.PackageModel",1,0,"PackageModel");
    qmlRegisterType<PamacQt::AURPackageModel>("Pamac.AUR.PackageModel",1,0,"AURPackageModel");
    qmlRegisterUncreatableType<PamacQt::Updates>("Pamac.Database",1,0,"Updates","");
    qmlRegisterType<PamacQt::Transaction>("Pamac.Transaction",1,0,"Transaction");
    qmlRegisterUncreatableType<PamacQt::Config>("Pamac.Config",1,0,"Config","");
    qmlRegisterUncreatableType<PamacQt::TransactionSummary>("Pamac.Transaction",1,0,"TransactionSummary","");

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


    return QApplication::exec();

}
