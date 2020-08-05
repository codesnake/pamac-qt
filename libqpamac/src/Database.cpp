#include "Database.h"
#include <functional>
#include <QFile>

namespace LibQPamac {
Database::Database(PamacDatabase *db, QObject *parent):
    QObject(parent)
{
}

Database::Database(const QString& configFile, QObject *parent):
    QObject(parent)
    {
    m_config = new Config(configFile,this);
    handle = pamac_database_new(m_config->handle());
    init();
}

//QFile Database::cloneBuildFiles(const QString &pkgname, bool overwrite) const{

//    auto gfile = pamac_database_clone_build_files(handle,pkgname.toUtf8(),gboolean(overwrite),nullptr);

//    return QFile(g_file_get_path(gfile));
//}

QStringList Database::getRepos()
{
    return Utils::gsListToQStringList(pamac_database_get_repos_names(handle),true);
}

QStringList Database::getGroups()
{
    return Utils::gsListToQStringList(pamac_database_get_groups_names(handle),true);
}

QVariantList Database::searchPkgsInAur(const QString &name)
{

    return Utils::gSListToQList<QVariant>(pamac_database_search_aur_pkgs(handle,name.toUtf8()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AurPackage));
}

QVariantList Database::getCategoryPackages(const QString &category)
{
    return Utils::gSListToQList<QVariant>(pamac_database_get_category_pkgs(handle,category.toUtf8()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
}

QVariantList Database::searchPkgs(const QString &name)
{
    return Utils::gSListToQList<QVariant>(pamac_database_search_pkgs(handle,name.toUtf8()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
}

QVariantList Database::getGroupPackages(const QString &group)
{
    return Utils::gSListToQList<QVariant>(pamac_database_get_group_pkgs(handle,group.toUtf8()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
}

QVariantList Database::getRepoPackages(const QString &repo)
{
    return Utils::gSListToQList<QVariant>(pamac_database_get_repo_pkgs(handle,repo.toUtf8()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));

}

QVariantList Database::getInstalledApps(){
    return Utils::gSListToQList<QVariant>(pamac_database_get_installed_apps(handle),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
}

QStringList Database::getPkgFiles(const QString &name){
    return  Utils::gsListToQStringList(pamac_database_get_pkg_files(handle,name.toUtf8()),true);
}

QStringList Database::getMirrorsCountries(){
    QStringList result = Utils::gsListToQStringList(pamac_database_get_mirrors_countries(handle),true);
    result.push_front("Worldwide");
    return result;
}

QString Database::getMirrorsChoosenCountry(){
    gchar* res = pamac_database_get_mirrors_choosen_country(handle);
    QString result = QString::fromUtf8(res);
    g_free(res);
    return result;

}

Updates Database::getUpdates(){
    return {pamac_database_get_updates(handle, false)};
}

AlpmPackage Database::getInstalledPackage(const QString &name){
    return {pamac_database_get_installed_pkg(handle,name.toUtf8())};
}

AlpmPackage Database::getSyncPackage(const QString &name){
    return {pamac_database_get_sync_pkg(handle,name.toUtf8())};
}

AurPackage Database::getAurPackage(const QString &name){
    return {pamac_database_get_aur_pkg(handle,name.toUtf8())};
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

void Database::setConfig(Config *config)
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

QVariantList LibQPamac::Database::getInstalledPackages(Database::InstalledPackageTypes type){
    GSList* result;
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

    return LibQPamac::Utils::gSListToQList<QVariant>(result,PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
}



QVariantList Database::getAurPackages(const QStringList &nameList)
{

    auto list = new std::vector<char*>(Utils::qStringListToCStringVector(nameList));

    auto resultGlist = pamac_database_get_aur_pkgs(handle,list->data(),list->size());
    QVariantList result;
    for(auto el = g_hash_table_get_values(resultGlist);el!=nullptr;el=el->next){
        result.append(QVariant::fromValue(AurPackage::fromData(el->data)));
    }
    return result;
}

AlpmPackage Database::getPkg(const QString &name)
{
   return {pamac_database_get_pkg(handle,name.toUtf8())};
}

} // namespace LibQPamac
