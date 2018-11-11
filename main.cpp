#include "pamac.h"
#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <Database.h>
#include <QJSValue>
#include "PackageModel.h"
#include "Updates.h"
#include "Transaction.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qRegisterMetaType<PamacQt::PackageList>("PackageList");
    qRegisterMetaType<PamacQt::PackageDetails>("PackageDetails");
    qRegisterMetaType<PamacQt::Updates>("Updates");
    qRegisterMetaType<PamacQt::Config>("Config");




    qmlRegisterSingletonType<PamacQt::Database>("Pamac.alpm.database",1,0,"Database",[](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        PamacQt::Database *database = new PamacQt::Database("/etc/pamac.conf",engine);
        PamacQt::Transaction tr(database,engine);
        tr.unlock();
        return database;
    });

    qmlRegisterUncreatableType<PamacQt::PackageDetails>("Pamac.alpm.package",1,0,"PackageDetails","");
    qmlRegisterType<PamacQt::PackageModel>("Pamac.alpm.packageModel",1,0,"PackageModel");
    qmlRegisterUncreatableType<PamacQt::PackageList>("Pamac.alpm.package",1,0,"PackageList","");
    qmlRegisterUncreatableType<PamacQt::Updates>("Pamac.alpm.database",1,0,"Updates","");
    qmlRegisterType<PamacQt::Transaction>("Pamac.alpm.transaction",1,0,"Transaction");
    qmlRegisterUncreatableType<PamacQt::Config>("Pamac.alpm.config",1,0,"Config","");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();

}
