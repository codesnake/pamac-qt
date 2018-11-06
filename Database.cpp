#include "Database.h"
namespace PamacQt {
Database::Database(PamacDatabase *db, QObject *parent):
    QObject(parent),
    db(std::shared_ptr<PamacDatabase>(db,g_object_unref)){
    pamac_database_enable_appstream(db);
}

Database::Database(QString configFile, QObject *parent):
    QObject(parent){
    db = std::shared_ptr<PamacDatabase>(pamac_database_new(pamac_config_new(configFile.toUtf8())),g_object_unref);
    pamac_database_enable_appstream(db.get());
}

PackageList Database::getInstalledPackages(Database::InstalledPackageTypes type)
{
    switch (type) {
    case Installed:
        return PackageList(pamac_database_get_installed_pkgs(db.get()));
    case Explicitly:
        return PackageList(pamac_database_get_explicitly_installed_pkgs(db.get()));
    case Orphans:
        return PackageList(pamac_database_get_orphans(db.get()));
    case Foreign:
        return PackageList(pamac_database_get_foreign_pkgs(db.get()));
    }
}

QStringList Database::getRepos()
{
    QStringList result;
    auto list = pamac_database_get_repos_names(db.get());

    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(QString::fromUtf8(static_cast<char*>(el->data)));
    }
    g_list_free_full(list,g_free);

    return result;
}

QStringList Database::getGroups()
{
    QStringList result;

    auto list = pamac_database_get_groups_names(db.get());

    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(QString::fromUtf8(static_cast<char*>(el->data)));
    }
    g_list_free_full(list,g_free);

    return result;
}

QStringList Database::getPkgFiles(const QString &name){
    QStringList result;
    auto list = pamac_database_get_pkg_files(db.get(),name.toUtf8());
    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(QString::fromUtf8(static_cast<char*>(el->data)));
    }

    g_list_free_full(list,g_free);

    return result;
}

void Database::getUpdatesAsync(){

    pamac_database_get_updates(db.get(),[](GObject* obj, GAsyncResult* cb,void* data){
        auto db = reinterpret_cast<Database*>(data);
        Updates arg(pamac_database_get_updates_finish(
                        reinterpret_cast<PamacDatabase*>(obj),
                        cb));
        emit db->updatesReady(arg);

    },this);


}

} // namespace PamacQt
