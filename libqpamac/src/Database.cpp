#include "Database.h"

#include <Database.h>

namespace LibQPamac {
Database::Database(PamacDatabase *db, QObject *parent):
    QObject(parent),
    m_db(std::shared_ptr<PamacDatabase>(db,g_object_unref)){
    init();
}

Database::Database(const QString& configFile, QObject *parent):
    QObject(parent){
    m_config = Config(configFile);
    m_db = std::shared_ptr<PamacDatabase>(pamac_database_new(m_config),g_object_unref);
    init();
}

RepoPackageDetails Database::getPkgDetails(const QString &pkgname, const QString &app_name, bool useSyncDB)
{
    return RepoPackageDetails(pamac_database_get_pkg_details(m_db.get(),pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
}

GenericQmlFuture Database::getAurPkgDetails(const QString &pkgname)
{
    auto future = new QmlFutureImpl;
    pamac_database_get_aur_pkg_details(m_db.get(),pkgname.toUtf8(),PAMAC_QT_AUR_PACKAGE_DETAILS_ASYNC_CALLBACK(pamac_database_get_aur_pkg_details_finish),future);
    return GenericQmlFuture(future);

}

GenericQmlFuture Database::cloneBuildFiles(const QString &pkgname, bool overwrite){
    auto future = new QmlFutureImpl;
    pamac_database_clone_build_files(m_db.get(),pkgname.toUtf8(),overwrite,
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
    return RepoPackageList::fromGList(pamac_database_get_installed_apps(m_db.get()));
}

RepoPackageList Database::getInstalledPackages(Database::InstalledPackageTypes type)
{
    RepoPackageList lst;
    switch (type) {
    case Installed:
        lst = RepoPackageList::fromGList(pamac_database_get_installed_pkgs(m_db.get()));
        break;
    case Explicitly:
        lst = RepoPackageList::fromGList(pamac_database_get_explicitly_installed_pkgs(m_db.get()));
        break;
    case Orphans:
        lst = RepoPackageList::fromGList(pamac_database_get_orphans(m_db.get()));
        break;
    case Foreign:
        lst =  RepoPackageList::fromGList(pamac_database_get_foreign_pkgs(m_db.get()));
        break;
    }
    return lst;
}

QStringList Database::getRepos()
{
    return Utils::gListToQStringList(pamac_database_get_repos_names(m_db.get()),true);
}

QStringList Database::getGroups()
{
    return Utils::gListToQStringList(pamac_database_get_groups_names(m_db.get()),true);
}

RepoPackageList Database::getCategoryPackages(const QString &category)
{
    return RepoPackageList::fromGList(pamac_database_get_category_pkgs(m_db.get(),category.toUtf8()));
}

RepoPackageList Database::getRepoPackages(const QString &repo)
{
    return RepoPackageList::fromGList(pamac_database_get_repo_pkgs(m_db.get(),repo.toUtf8()));
}

RepoPackageList Database::getGroupPackages(const QString &group)
{
    return RepoPackageList::fromGList(pamac_database_get_group_pkgs(m_db.get(),group.toUtf8()));
}

QStringList Database::getIgnorePkgs()
{
    return Utils::gListToQStringList(pamac_database_get_ignorepkgs(m_db.get()),true);
}

GenericQmlFuture Database::searchPkgsInAurAsync(const QString &name)
{

    auto future = new QmlFutureImpl;

    pamac_database_search_in_aur(m_db.get(),name.toUtf8(),PAMAC_QT_AUR_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_in_aur_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getCategoryPackagesAsync(const QString &category)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_category_pkgs_async(m_db.get(),category.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_category_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::searchPkgsAsync(const QString &name)
{
    auto future = new QmlFutureImpl;

    pamac_database_search_pkgs_async(m_db.get(),name.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getGroupPackagesAsync(const QString &group)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_group_pkgs_async(m_db.get(),group.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_group_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getRepoPackagesAsync(const QString &repo)
{
    auto future = new QmlFutureImpl;

    pamac_database_get_repo_pkgs_async(m_db.get(),repo.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_repo_pkgs_finish),future);
    return GenericQmlFuture(future);
}

GenericQmlFuture Database::getInstalledAppsAsync(){
    auto future = new QmlFutureImpl;
    pamac_database_get_installed_apps_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_installed_apps_finish),future);
    return GenericQmlFuture(future);
}

QStringList Database::getPkgFiles(const QString &name){
    return  Utils::gListToQStringList(pamac_database_get_pkg_files(m_db.get(),name.toUtf8()),true);
}

RepoPackageList Database::searchPkgs(const QString &name)
{
    return RepoPackageList::fromGList(pamac_database_search_pkgs(m_db.get(),name.toUtf8()));
}

QStringList Database::getMirrorsCountries(){
    QStringList result = Utils::gListToQStringList(pamac_database_get_mirrors_countries(m_db.get()),true);
    result.push_front("Worldwide");
    return result;
}

QString Database::getMirrorsChoosenCountry(){
    gchar* res = pamac_database_get_mirrors_choosen_country(m_db.get());
    QString result = QString::fromUtf8(res);
    g_free(res);
    return result;

}

void Database::getUpdatesAsync(){

    pamac_database_get_updates(m_db.get(),[](GObject* obj, GAsyncResult* cb,void* data){
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
    return {pamac_database_get_installed_pkg(m_db.get(),name.toUtf8())};
}

RepoPackage Database::getSyncPackage(const QString &name){
    return {pamac_database_get_sync_pkg(m_db.get(),name.toUtf8())};
}

GenericQmlFuture Database::getAurPackage(const QString &name){
    auto future = new QmlFutureImpl;
    pamac_database_get_aur_pkg(m_db.get(),name.toUtf8(),[](GObject* object,GAsyncResult* result,gpointer futurePtr){
        auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);
        if(future->isRunning()) {
            future->setFuture(QVariant::fromValue(AURPackage(pamac_database_get_aur_pkg_finish(reinterpret_cast<PamacDatabase*>(object),result))));
        } else {
            delete future;
        }
    },future);

    return GenericQmlFuture(future);
}



bool Database::getCheckspace() const
{
    return pamac_database_get_checkspace(m_db.get());
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
    pamac_database_enable_appstream(m_db.get());

    g_signal_connect(m_db.get(),"get_updates_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,uint percent,Database* t){
                         Q_UNUSED(obj);
                         Q_EMIT t->getUpdatesProgress(percent);
                     }),this);
}

GenericQmlFuture LibQPamac::Database::getInstalledPackagesAsync(Database::InstalledPackageTypes type){
    auto future = new QmlFutureImpl;
    switch (type) {
    case Installed:
        pamac_database_get_installed_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_installed_pkgs_finish),future);
        break;
    case Explicitly:
        pamac_database_get_explicitly_installed_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_explicitly_installed_pkgs_finish),future);
        break;
    case Orphans:
        pamac_database_get_orphans_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_orphans_finish),future);
        break;
    case Foreign:
        pamac_database_get_foreign_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_foreign_pkgs_finish),future);
        break;
    }

    return GenericQmlFuture(future);
}

} // namespace LibQPamac
