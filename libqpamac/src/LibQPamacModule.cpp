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

void LibQPamacModule::registerTypes(const char *uri)
{
    using namespace LibQPamac;
    qRegisterMetaType<AurPackage>("AURPackage");
    qRegisterMetaType<AlpmPackage>("AlpmPackage");
    qRegisterMetaType<Package>("Package");
    qRegisterMetaType<Updates>("Updates");
    qRegisterMetaType<Config>("Config");
    qRegisterMetaType<TransactionSummary>("TransactionSummary");
    qRegisterMetaType<GenericQmlFuture>("Future");
    qRegisterMetaType<QList<HistoryItem>>("QList<HistoryItem>");

    qmlRegisterSingletonType<LibQPamac::Database>("QPamac.Database",1,0,"Database",
                                                [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(scriptEngine)

        return new LibQPamac::Database("/etc/pamac.conf",scriptEngine);
    });

    qmlRegisterUncreatableType<GenericQmlFuture>("QPamac.Async",1,0,"Future","");
    qmlRegisterType<QmlFutureWatcher>("QPamac.Async",1,0,"FutureWatcher");

    qmlRegisterType<HistoryItemModel>("QPamac.History",1,0,"HistoryModel");
    qmlRegisterUncreatableType<AurPackage>("QPamac.AUR.Package",1,0,"AURPackage","");
    qmlRegisterUncreatableType<AlpmPackage>("QPamac.Package",1,0,"AlpmPackage","");
    qmlRegisterType<Package>();
    qmlRegisterType<PackageModel>("QPamac.PackageModel",1,0,"PackageModel");
    qmlRegisterType<AurPackageModel>("QPamac.AUR.PackageModel",1,0,"AURPackageModel");
    qmlRegisterUncreatableType<Updates>("QPamac.Database",1,0,"Updates","");
    qmlRegisterType<Transaction>("QPamac.Transaction",1,0,"Transaction");
    qmlRegisterUncreatableType<Config>("QPamac.Config",1,0,"Config","");
    qmlRegisterUncreatableType<TransactionSummary>("QPamac.Transaction",1,0,"TransactionSummary","");
}
