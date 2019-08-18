#include "LibQPamacModule.h"
#include <QQmlEngine>
#include <AsyncHelpers.h>
#include <Database.h>
#include <AurPackageModel.h>
#include <AurPackage.h>
#include <Package.h>
#include <PackageModel.h>
#include <Updates.h>
#include <Config.h>
#include <Transaction.h>
#include <QMetaType>
Q_DECLARE_METATYPE(QList<LibQPamac::Package>);

void LibQPamacModule::registerTypes(const char *uri)
{
    using namespace LibQPamac;
    qRegisterMetaType<LibQPamac::AURPackage>("AURPackage");
    qRegisterMetaType<LibQPamac::PackageDetails>("PackageDetails");
    qRegisterMetaType<LibQPamac::Package>("Package");
    qRegisterMetaType<LibQPamac::Updates>("Updates");
    qRegisterMetaType<LibQPamac::Config>("Config");
    qRegisterMetaType<LibQPamac::TransactionSummary>("TransactionSummary");
    qRegisterMetaType<GenericQmlFuture>("Future");
    qRegisterMetaType<QList<LibQPamac::HistoryItem>>("QList<HistoryItem>");

    qmlRegisterSingletonType<LibQPamac::Database>("QPamac.Database",1,0,"Database",
                                                [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(scriptEngine)

        LibQPamac::Database *database = new LibQPamac::Database("/etc/pamac.conf",scriptEngine);
        return database;
    });

    qmlRegisterUncreatableType<GenericQmlFuture>("QPamac.Async",1,0,"Future","");
    qmlRegisterType<QmlFutureWatcher>("QPamac.Async",1,0,"FutureWatcher");

    qmlRegisterType<LibQPamac::HistoryItemModel>("QPamac.History",1,0,"HistoryModel");
    qmlRegisterUncreatableType<LibQPamac::AURPackage>("QPamac.AUR.Package",1,0,"AURPackage","");
    qmlRegisterUncreatableType<LibQPamac::PackageDetails>("QPamac.Package",1,0,"PackageDetails","");
    qmlRegisterUncreatableType<LibQPamac::Package>("QPamac.Package",1,0,"Package","");
    qmlRegisterType<LibQPamac::PackageModel>("QPamac.PackageModel",1,0,"PackageModel");
    qmlRegisterType<LibQPamac::AurPackageModel>("QPamac.AUR.PackageModel",1,0,"AURPackageModel");
    qmlRegisterUncreatableType<LibQPamac::Updates>("QPamac.Database",1,0,"Updates","");
    qmlRegisterType<LibQPamac::Transaction>("QPamac.Transaction",1,0,"Transaction");
    qmlRegisterUncreatableType<LibQPamac::Config>("QPamac.Config",1,0,"Config","");
    qmlRegisterUncreatableType<LibQPamac::TransactionSummary>("QPamac.Transaction",1,0,"TransactionSummary","");
}
