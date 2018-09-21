#include "pamac.h"

#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <AlpmDB.h>
#include <QJSValue>
#include "AlpmPackagesModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterSingletonType<AlpmDB>("Pamaq.alpm.database",1,0,"Database",[](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        AlpmDB *database = new AlpmDB("/etc/pacman.conf");
        return database;
    });
    qmlRegisterUncreatableType<AlpmPkgDetails>("Pamaq.alpm.package",1,0,"AlpmPkgDetails","");

    qmlRegisterType<PackageModel>("Pamaq.alpm.packageModel",1,0,"AlpmPackageModel");
    qmlRegisterUncreatableType<AlpmPackageList>("Pamaq.alpm.package",1,0,"AlpmPackageList","");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QQmlContext *ctxt = engine.rootContext();


    return app.exec();
}
