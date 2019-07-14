#include "Database.h"

template <typename T>
auto futureWrapFunction = [](QmlFutureImpl* future,std::function<T> function){
future->mutex.lock();
if(future->isRunning()){
future->setFuture(QVariant::fromValue(function()));
} else {
delete future;
}
future->mutex.unlock();
};

namespace LibQPamac {
Database::Database(PamacDatabase *db, QObject *parent):
    QObject(parent)
   {

    init();
}

Database::Database(const QString& configFile, QObject *parent):
    QObject(parent){
    m_config = Config(configFile);
    handle = pamac_database_new(&m_config);
    init();
}

RepoPackageDetails Database::getPkgDetails(const QString &pkgname, const QString &app_name, bool useSyncDB)
{
    return RepoPackageDetails(pamac_database_get_pkg_details(handle,pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
}

GenericQmlFuture Database::getAurPkgDetails(const QString &pkgname)
{
    auto future = new QmlFutureImpl;
    pamac_database_get_aur_pkg_details(handle,pkgname.toUtf8(),Utils::cify([](GObject* parent,GAsyncResult* result,void* future){
        return  futureWrapFunction<AURPackageDetails()>(reinterpret_cast<QmlFutureImpl*>(future),[=](){
            return AURPackageDetails(pamac_database_get_aur_pkg_details_finish(reinterpret_cast<PamacDatabase*>(parent),result));
        });
    }),future);
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

RepoPackageList Database::getInstalledApps()
{
    return RepoPackageList::fromGList(pamac_database_get_installed_apps(handle));
}

RepoPackageList Database::getInstalledPackages(Database::InstalledPackageTypes type)
{
    RepoPackageList lst;
    switch (type) {
    case Installed:
        lst = RepoPackageList::fromGList(pamac_database_get_installed_pkgs(handle));
        break;
    case Explicitly:
        lst = RepoPackageList::fromGList(pamac_database_get_explicitly_installed_pkgs(handle));
        break;
    case Orphans:
        lst = RepoPackageList::fromGList(pamac_database_get_orphans(handle));
        break;
    case Foreign:
        lst =  RepoPackageList::fromGList(pamac_database_get_foreign_pkgs(handle));
        break;
    }
    return lst;
}

QStringList Database::getRepos()
{
    return Utils::gListToQStringList(pamac_database_get_repos_names(handle),true);
}

QStringList Database::getGroups()
{
    return Utils::gListToQStringList(pamac_database_get_groups_names(handle),true);
}

RepoPackageList Database::getCategoryPackages(const QString &category)
{
    return RepoPackageList::fromGList(pamac_database_get_category_pkgs(handle,category.toUtf8()));
}

RepoPackageList Database::getRepoPackages(const QString &repo)
{
    return RepoPackageList::fromGList(pamac_database_get_repo_pkgs(handle,repo.toUtf8()));
}

RepoPackageList Database::getGroupPackages(const QString &group)
{
    return RepoPackageList::fromGList(pamac_database_get_group_pkgs(handle,group.toUtf8()));
}

QStringList Database::getIgnorePkgs()
{
    return Utils::gListToQStringList(pamac_database_get_ignorepkgs(handle),true);
}

GenericQmlFuture Database::searchPkgsInAurAsync(const QString &name)
{
    auto future = new QmlFutureImpl;

    pamac_database_search_in_aur(handle,name.toUtf8(),PAMAC_QT_AUR_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_in_aur_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getCategoryPackagesAsync(const QString &category)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_category_pkgs_async(handle,category.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_category_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::searchPkgsAsync(const QString &name)
{
    auto future = new QmlFutureImpl;

    pamac_database_search_pkgs_async(handle,name.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getGroupPackagesAsync(const QString &group)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_group_pkgs_async(handle,group.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_group_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getRepoPackagesAsync(const QString &repo)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_repo_pkgs_async(handle,repo.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_repo_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getInstalledAppsAsync(){
    auto future = new QmlFutureImpl;
    pamac_database_get_installed_apps_async(handle,([](GObject* parent,GAsyncResult* result,void* future){
        return  futureWrapFunction<RepoPackageList()>(reinterpret_cast<QmlFutureImpl*>(future),[=](){
            return RepoPackageList::fromGList(pamac_database_get_installed_apps_finish(reinterpret_cast<PamacDatabase*>(parent),result));
        });
    }),future);
    return GenericQmlFuture(future);
}

QStringList Database::getPkgFiles(const QString &name){
    return  Utils::gListToQStringList(pamac_database_get_pkg_files(handle,name.toUtf8()),true);
}

RepoPackageList Database::searchPkgs(const QString &name)
{
    return RepoPackageList::fromGList(pamac_database_search_pkgs(handle,name.toUtf8()));
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

RepoPackageList Database::getPending(const QStringList &toInstall, const QStringList &toRemove)
{
    RepoPackageList result;
    for (const auto &element: toInstall) {
        RepoPackage pkg;
        if((pkg = getInstalledPackage(element)).name().isEmpty()){
            pkg = getSyncPackage(element);
        }
        result.append(pkg);
    }
    for(const auto &element:toRemove){
        RepoPackage pkg;
        if(!(pkg = getInstalledPackage(element)).name().isEmpty()){
            result.append(pkg);
        }
    }
    return result;
}

RepoPackage Database::getInstalledPackage(const QString &name){
    return {pamac_database_get_installed_pkg(handle,name.toUtf8())};
}

RepoPackage Database::getSyncPackage(const QString &name){
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
        pamac_database_get_installed_pkgs_async(handle,PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_installed_pkgs_finish),future);
        break;
    case Explicitly:
        pamac_database_get_explicitly_installed_pkgs_async(handle,PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_explicitly_installed_pkgs_finish),future);
        break;
    case Orphans:
        pamac_database_get_orphans_async(handle,PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_orphans_finish),future);
        break;
    case Foreign:
        pamac_database_get_foreign_pkgs_async(handle,PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_foreign_pkgs_finish),future);
        break;
    }

    return GenericQmlFuture(future);
}

} // namespace LibQPamac
