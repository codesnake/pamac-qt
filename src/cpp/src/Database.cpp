#include "Database.h"

namespace PamacQt {
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

PackageList Database::getInstalledPackages(Database::InstalledPackageTypes type)
{
    switch (type) {
    case Installed:
        return PackageList(pamac_database_get_installed_pkgs(m_db.get()));
    case Explicitly:
        return PackageList(pamac_database_get_explicitly_installed_pkgs(m_db.get()));
    case Orphans:
        return PackageList(pamac_database_get_orphans(m_db.get()));
    case Foreign:
        return PackageList(pamac_database_get_foreign_pkgs(m_db.get()));
    }
}

QStringList Database::getRepos()
{
    return Utils::gListToQStringList(pamac_database_get_repos_names(m_db.get()),true);
}

QStringList Database::getGroups()
{
    return Utils::gListToQStringList(pamac_database_get_groups_names(m_db.get()),true);
}

QStringList Database::getPkgFiles(const QString &name){
    return  Utils::gListToQStringList(pamac_database_get_pkg_files(m_db.get(),name.toUtf8()),true);
}

void Database::getUpdatesAsync(){

    pamac_database_get_updates(m_db.get(),[](GObject* obj, GAsyncResult* cb,void* data){
        auto db = reinterpret_cast<Database*>(data);
        Updates arg(pamac_database_get_updates_finish(
                        reinterpret_cast<PamacDatabase*>(obj),
                        cb));
        emit db->updatesReady(arg);
    },this);

}

void Database::init()
{
    pamac_database_enable_appstream(m_db.get());

    g_signal_connect(m_db.get(),"get_updates_progress",
                     reinterpret_cast<GCallback>(+[](GObject* obj,uint percent,Database* t){
                         emit t->getUpdatesProgress(percent);
                     }),this);
}

} // namespace PamacQt
