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

//QFile Database::cloneBuildFiles(const QString &pkgname, bool overwrite) const{

//    auto gfile = pamac_database_clone_build_files(handle,pkgname.toUtf8(),gboolean(overwrite),nullptr);

//    return QFile(g_file_get_path(gfile));
//}

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
//    return Utils::gListToQStringList(pamac_database_get_ignorepkgs(handle),true);
}

QList<AurPackage> Database::searchPkgsInAurAsync(const QString &name)
{

    return Utils::gListToQList<AurPackage>(pamac_database_search_aur_pkgs(handle,name.toUtf8()),[](void* data){return AurPackage(data);});
}

QList<AlpmPackage> Database::getCategoryPackagesAsync(const QString &category)
{
    return Utils::gListToQList<AlpmPackage>(pamac_database_get_category_pkgs(handle,category.toUtf8()),[](void* data){return AlpmPackage(data);});
}

QList<AlpmPackage> Database::searchPkgs(const QString &name)
{
    return Utils::gListToQList<AlpmPackage>(pamac_database_search_pkgs(handle,name.toUtf8()),[](void* data){return AlpmPackage(data);});
}

QList<AlpmPackage> Database::getGroupPackagesAsync(const QString &group)
{
    return Utils::gListToQList<AlpmPackage>(pamac_database_get_group_pkgs(handle,group.toUtf8()),[](void* data){return AlpmPackage(data);});
}

QList<AlpmPackage> Database::getRepoPackagesAsync(const QString &repo)
{
    return Utils::gListToQList<AlpmPackage>(pamac_database_get_repo_pkgs(handle,repo.toUtf8()),[](void* data){return AlpmPackage(data);});

}

QList<AlpmPackage> Database::getInstalledAppsAsync(){
    return Utils::gListToQList<AlpmPackage>(pamac_database_get_installed_apps(handle),[](void* data){return AlpmPackage(data);});
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

Updates Database::getUpdatesAsync(){

    return Updates(pamac_database_get_updates(handle));

}

Package Database::getInstalledPackage(const QString &name){
    return {pamac_database_get_installed_pkg(handle,name.toUtf8())};
}

Package Database::getSyncPackage(const QString &name){
    return {pamac_database_get_sync_pkg(handle,name.toUtf8())};
}

AurPackage Database::getAurPackage(const QString &name){
    return AurPackage(pamac_database_get_aur_pkg(handle,name.toUtf8()));
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

QList<QVariant> Database::getHistory(){
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

QList<AlpmPackage> LibQPamac::Database::getInstalledPackagesAsync(Database::InstalledPackageTypes type){
    GList* result;
    switch (type) {
    case Installed:
        result = pamac_database_get_installed_pkgs(handle);
        break;
    case Explicitly:
        result = pamac_database_get_explicitly_installed_pkgs(handle);
        break;
    case Orphans:
        result = pamac_database_get_orphans(handle);
        break;
    case Foreign:
        result = pamac_database_get_foreign_pkgs(handle);
        break;
    }

    return LibQPamac::Utils::gListToQList<AlpmPackage>(result,[](void* value)->AlpmPackage{return AlpmPackage(value);});
}



QList<AurPackage> Database::getAurPackages(const QStringList &nameList)
{

    auto list = new std::vector<char*>(Utils::qStringListToCStringVector(nameList));

    auto resultGlist = pamac_database_get_aur_pkgs(handle,list->data(),list->size());
    QList<AurPackage> result;
    for(auto el = g_hash_table_get_values(resultGlist);el!=nullptr;el=el->next){
        result.append(AurPackage(el->data));
    }
    return result;
}

} // namespace LibQPamac
