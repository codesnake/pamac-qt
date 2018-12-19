#include "pamac.h"
#include <QQmlContext>
#include <QApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <Database.h>
#include <QJSValue>
#include <QIcon>
#include "PackageModel.h"
#include "Updates.h"
#include "Transaction.h"
#include "XDGIconProvider.h"
#include "Utils.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    app.setOrganizationName("Artem Grinev");
    app.setApplicationName("PamacQt");

    qRegisterMetaType<PamacQt::PackageList>("PackageList");
    qRegisterMetaType<PamacQt::PackageDetails>("PackageDetails");
    qRegisterMetaType<PamacQt::Package>("Package");
    qRegisterMetaType<PamacQt::Updates>("Updates");
    qRegisterMetaType<PamacQt::Config>("Config");
    qRegisterMetaType<PamacQt::TransactionSummary>("TransactionSummary");


    //A (dirty) work around the bug that causes icons not to load in kde: manually set icon theme name
    {
        QIcon::setThemeName("breeze");
    }

    qmlRegisterSingletonType<PamacQt::Database>("Pamac.Database",1,0,"Database",
                                                [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        PamacQt::Database *database = new PamacQt::Database("/etc/pamac.conf",engine);
        return database;
    });

    qmlRegisterUncreatableType<PamacQt::PackageDetails>("Pamac.Package",1,0,"PackageDetails","");
    qmlRegisterUncreatableType<PamacQt::Package>("Pamac.Package",1,0,"Package","");
    qmlRegisterType<PamacQt::PackageModel>("Pamac.PackageModel",1,0,"PackageModel");
    qmlRegisterUncreatableType<PamacQt::PackageList>("Pamac.Package",1,0,"PackageList","");
    qmlRegisterUncreatableType<PamacQt::Updates>("Pamac.Database",1,0,"Updates","");
    qmlRegisterType<PamacQt::Transaction>("Pamac.Transaction",1,0,"Transaction");
    qmlRegisterUncreatableType<PamacQt::Config>("Pamac.Config",1,0,"Config","");
    qmlRegisterUncreatableType<PamacQt::TransactionSummary>("Pamac.Transaction",1,0,"TransactionSummary","");

    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("icons"), new XDGIconProvider);
    engine.load(QUrl(QStringLiteral("qrc:/src/qml/MainWindow.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();

}
