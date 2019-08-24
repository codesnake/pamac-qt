#include "Database.h"
#include <functional>

GAsyncReadyCallback asyncCallback(){
    return ([](GObject* parent,GAsyncResult* result,void* lambdaPtr){

        auto function = reinterpret_cast<std::function<void(PamacDatabase*,GAsyncResult*)>*>(lambdaPtr);
        auto object = reinterpret_cast<PamacDatabase*>(parent);
        (*function)(object,result);
        delete function;
    });
}

namespace LibQPamac {
Database::Database(PamacDatabase *db, QObject *parent):
    QObject(parent)
{
}

Database::Database(const QString& configFile, QObject *parent):
    QObject(parent){
    m_config = Config(configFile);
    handle = pamac_database_new(m_config);
    init();
}

PackageDetails Database::getPkgDetails(const QString &pkgname, const QString &app_name, bool useSyncDB)
{
    return PackageDetails(pamac_database_get_pkg_details(handle,pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
}

GenericQmlFuture Database::getAurPkgDetails(const QString &pkgname)
{
    auto future = new QmlFutureImpl;
    pamac_database_get_aur_pkg_details(handle,
                                       pkgname.toUtf8(),
                                       asyncCallback(),
                                       new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(QVariant::fromValue(AURPackageDetails(pamac_database_get_aur_pkg_details_finish(object,result))));
    }));
    return GenericQmlFuture(future);

}

GenericQmlFuture Database::cloneBuildFiles(const QString &pkgname, bool overwrite){
    auto future = new QmlFutureImpl;
    pamac_database_clone_build_files(handle,pkgname.toUtf8(),overwrite,
                                     [](GObject* parent,GAsyncResult* result,void* futurePtr){
        auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);

        auto folder = pamac_database_clone_build_files_finish(reinterpret_cast<PamacDatabase*>(parent),result);

        if(future->isRunning()){
            future->setFuture(QVariant(QString::fromUtf8(g_file_get_path(folder))));
        }   else {
            delete future;
        }
        g_object_unref(folder);
    }
    ,future);
    return GenericQmlFuture(future);
}

QStringList Database::getRepos()
{
    return Utils::gListToQStringList(pamac_database_get_repos_names(handle),true);
}

QStringList Database::getGroups()
{
    return Utils::gListToQStringList(pamac_database_get_groups_names(handle),true);
}

QStringList Database::getIgnorePkgs()
{
    return Utils::gListToQStringList(pamac_database_get_ignorepkgs(handle),true);
}

GenericQmlFuture Database::searchPkgsInAurAsync(const QString &name)
{
    auto future = new QmlFutureImpl;

    pamac_database_search_in_aur(handle,name.toUtf8(),
                                 asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(QVariant::fromValue
                          (LibQPamac::Utils::gListToQList<QVariant>(
                               pamac_database_search_in_aur_finish(object,result),
                               PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AURPackage))));
    }));
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getCategoryPackagesAsync(const QString &category)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_category_pkgs_async(handle,category.toUtf8(),asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_category_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
    }));
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::searchPkgsAsync(const QString &name)
{
    auto future = new QmlFutureImpl;

    pamac_database_search_pkgs_async(handle,name.toUtf8(),asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_search_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
    }));
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getGroupPackagesAsync(const QString &group)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_group_pkgs_async(handle,group.toUtf8(),asyncCallback(),
                                        new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_group_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
    }));
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getRepoPackagesAsync(const QString &repo)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_repo_pkgs_async(handle,repo.toUtf8(),asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_repo_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
    }));
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getInstalledAppsAsync(){
    auto future = new QmlFutureImpl;
    pamac_database_get_installed_apps_async(handle,
                                            asyncCallback(),
                                            new std::function([=](PamacDatabase* object, GAsyncResult* result){
        future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_installed_apps_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
    }));
    return GenericQmlFuture(future);
}

QStringList Database::getPkgFiles(const QString &name){
    return  Utils::gListToQStringList(pamac_database_get_pkg_files(handle,name.toUtf8()),true);
}

QStringList Database::getMirrorsCountries(){
    QStringList result = Utils::gListToQStringList(pamac_database_get_mirrors_countries(handle),true);
    result.push_front("Worldwide");
    return result;
}

QString Database::getMirrorsChoosenCountry(){
    gchar* res = pamac_database_get_mirrors_choosen_country(handle);
    QString result = QString::fromUtf8(res);
    g_free(res);
    return result;

}

void Database::getUpdatesAsync(){

    pamac_database_get_updates(handle,[](GObject* obj, GAsyncResult* cb,void* data){
        auto db = reinterpret_cast<Database*>(data);
        Updates arg(pamac_database_get_updates_finish(
                        reinterpret_cast<PamacDatabase*>(obj),
                        cb));
        Q_EMIT db->updatesReady(arg);
    },this);

}

Package Database::getInstalledPackage(const QString &name){
    return {pamac_database_get_installed_pkg(handle,name.toUtf8())};
}

Package Database::getSyncPackage(const QString &name){
    return {pamac_database_get_sync_pkg(handle,name.toUtf8())};
}

GenericQmlFuture Database::getAurPackage(const QString &name){
    auto future = new QmlFutureImpl;
    pamac_database_get_aur_pkg(handle,name.toUtf8(),[](GObject* object,GAsyncResult* result,gpointer futurePtr){
        auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);
        if(future->isRunning()) {
            future->setFuture(QVariant::fromValue(AURPackage(pamac_database_get_aur_pkg_finish(reinterpret_cast<PamacDatabase*>(object),result))));
        } else {
            delete future;
        }
    },future);

    return GenericQmlFuture(future);
}

QVariantList Database::findPackagesByName(const QStringList &names)
{
    QVariantList packages;
    for(auto& name: names){
        Package pkg;
        if((pkg = getInstalledPackage(name)).name().isEmpty()){
            if((pkg = getSyncPackage(name)).name().isEmpty()){

            }
        }
        packages.append(QVariant::fromValue(pkg));

    }
    return packages;
}



bool Database::getCheckspace()
{
    return bool(pamac_database_get_checkspace(handle));
}

QList<HistoryItem> Database::getHistory(){
    QFile file("/var/log/pacman.log");
    file.open(QFile::ReadOnly);
    QStringList list;
    while(!file.atEnd()){
        QString line = QString::fromUtf8(file.readLine());
        if(line.contains("[ALPM]")){
            list.append(line);
        }
    }
    file.close();

    return HistoryItem::fromStringList(list);
}

void Database::setConfig(const Config &config)
{
    m_config = config;
    Q_EMIT configChanged(config);
}


void Database::init()
{
    pamac_database_enable_appstream(handle);

    g_signal_connect(handle,"get_updates_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,uint percent,Database* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->getUpdatesProgress(percent);
                     }),this);
}

GenericQmlFuture LibQPamac::Database::getInstalledPackagesAsync(Database::InstalledPackageTypes type){
    auto future = new QmlFutureImpl;
    switch (type) {
    case Installed:
        pamac_database_get_installed_pkgs_async(handle,asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
            future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_installed_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
        }));
        break;
    case Explicitly:
        pamac_database_get_explicitly_installed_pkgs_async(handle,asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
            future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_explicitly_installed_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
        }));
        break;
    case Orphans:
        pamac_database_get_orphans_async(handle,asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
            future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_orphans_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
        }));
        break;
    case Foreign:
        pamac_database_get_foreign_pkgs_async(handle,asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){
            future->setFuture(LibQPamac::Utils::gListToQList<QVariant>(pamac_database_get_foreign_pkgs_finish(object,result),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(Package)));
        }));
        break;
    }

    return GenericQmlFuture(future);
}



GenericQmlFuture Database::getAurPackages(const QStringList &nameList)
{

    auto list = new std::vector<char*>(Utils::qStringListToCStringVector(nameList));
    auto future = new QmlFutureImpl;

    pamac_database_get_aur_pkgs(handle,list->data(),list->size(),asyncCallback(),new std::function([=](PamacDatabase* object, GAsyncResult* result){

        auto hashTable = pamac_database_get_aur_pkgs_finish(object,result);

        QVariantList resultList;

        auto glist = g_hash_table_get_values(hashTable);

        for(auto it = glist;it!=nullptr;it=it->next){
            resultList.push_back(QVariant::fromValue(AURPackage(it->data)));
        }

        delete list;
        future->setFuture(resultList);
    }));
    return GenericQmlFuture(future);
}

} // namespace LibQPamac
