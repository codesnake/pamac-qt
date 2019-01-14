#pragma once
#include "pamac.h"
#include <QObject>
#include <Package.h>
#include <PackageList.h>
#include <QtDebug>
#include <iostream>
#include <QJSValue>
#include <QJSEngine>
#include <QFile>
#include "Updates.h"
#include "Config.h"
#include "Utils.h"
namespace PamacQt {
class Database:public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(bool checkspace READ getCheckspace CONSTANT)
public:
    enum InstalledPackageTypes{
        Installed,
        Explicitly,
        Orphans,
        Foreign
    };
    Q_ENUM(InstalledPackageTypes)

    enum PackageTypes{
        Repos,
        AUR
    };
    Q_ENUM(PackageTypes)

    Database(PamacDatabase* m_db,QObject* parent = nullptr);
    Database(const QString &configFile, QObject* parent = nullptr);

    inline Q_INVOKABLE RepoPackageDetails getPkgDetails(const QString &pkgname,const QString &app_name = "",bool useSyncDB = false)
    {
        return RepoPackageDetails(pamac_database_get_pkg_details(m_db.get(),pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
    }

    inline Q_INVOKABLE PackageList getInstalledApps()
    {
        return PackageList(pamac_database_get_installed_apps(m_db.get()));
    }

    Q_INVOKABLE PackageList getInstalledPackages(InstalledPackageTypes type);
    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();

    inline Q_INVOKABLE PackageList getCategoryPackages(const QString &category)
    {
        return PackageList(pamac_database_get_category_pkgs(m_db.get(),category.toUtf8()));
    }
    inline Q_INVOKABLE PackageList getRepoPackages(const QString &repo)
    {
        return PackageList(pamac_database_get_repo_pkgs(m_db.get(),repo.toUtf8()));
    }
    inline Q_INVOKABLE PackageList getGroupPackages(const QString &group)
    {
        return PackageList(pamac_database_get_group_pkgs(m_db.get(),group.toUtf8()));
    }
    inline Q_INVOKABLE QStringList getIgnorePkgs()
    {
        return Utils::gListToQStringList(pamac_database_get_ignorepkgs(m_db.get()),true);
    }

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);
    inline Q_INVOKABLE PackageList searchPkgs(const QString & name)
    {
        return PackageList(pamac_database_search_pkgs(m_db.get(),name.toUtf8()));
    }



    Q_INVOKABLE QStringList getMirrorsCountries(){
        QStringList result = Utils::gListToQStringList(pamac_database_get_mirrors_countries(m_db.get()),true);
        result.push_front("Worldwide");
        return result;
    }
    Q_INVOKABLE QString getMirrorsChoosenCountry(){
        gchar* res = pamac_database_get_mirrors_choosen_country(m_db.get());
        QString result = QString::fromUtf8(res);
        g_free(res);
        return result;

    }

    Q_INVOKABLE void getUpdatesAsync();

    Q_INVOKABLE PackageList getPending(const QStringList& toInstall, const QStringList& toRemove);

    Q_INVOKABLE RepoPackage getInstalledPackage(const QString& name){
        return RepoPackage(pamac_database_get_installed_pkg(m_db.get(),name.toUtf8()));
    }

    Q_INVOKABLE RepoPackage getSyncPackage(const QString& name){
        return RepoPackage(pamac_database_get_sync_pkg(m_db.get(),name.toUtf8()));
    }

    inline operator PamacDatabase*(){return m_db.get();}

    Config config() const
    {
        return m_config;
    }

    bool getCheckspace() const
    {
        return pamac_database_get_checkspace(m_db.get());
    }

    inline void refresh(){
        pamac_database_refresh(m_db.get());
    }
    Q_INVOKABLE QString getHistory(){
        QFile file("/var/log/pacman.log");
        file.open(QFile::ReadOnly);
        QString log =  QString::fromUtf8(file.readAll());
        file.close();
        return log;
    }

public slots:
    void setConfig(const Config& config)
    {
        m_config = config;
        emit configChanged(config);
    }



signals:
    void updatesReady(Updates upds);
    void getUpdatesProgress(uint percent);


    void checkspaceChanged(bool checkspace);

    void configChanged(Config config);

private:
    void init();
    std::shared_ptr<PamacDatabase> m_db;
    Config m_config;
    bool m_checkspace;
};
} //namespace PamacQt
