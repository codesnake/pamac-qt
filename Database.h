#pragma once
#include "pamac.h"
#include <QObject>
#include <Package.h>
#include <PackageList.h>
#include <QtDebug>
#include <iostream>
#include <QJSValue>
#include <QJSEngine>
#include "Updates.h"
#include "Config.h"
namespace PamacQt {
class Database:public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(bool checkspace READ getCheckspace)
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

    Database(PamacDatabase* db,QObject* parent = nullptr);
    Database(QString configFile,QObject* parent = nullptr);

    inline Q_INVOKABLE PackageDetails getPkgDetails(const QString &pkgname,const QString &app_name = "",bool useSyncDB = false)
    {
        return PackageDetails(pamac_database_get_pkg_details(db.get(),pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
    }

    inline Q_INVOKABLE PackageList getInstalledApps()
    {
        return PackageList(pamac_database_get_installed_apps(db.get()));
    }

    Q_INVOKABLE PackageList getInstalledPackages(InstalledPackageTypes type);
    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();

    inline Q_INVOKABLE PackageList getCategoryPackages(const QString &category)
    {
        return PackageList(pamac_database_get_category_pkgs(db.get(),category.toUtf8()));
    }
    inline Q_INVOKABLE PackageList getRepoPackages(const QString &repo)
    {
        return PackageList(pamac_database_get_repo_pkgs(db.get(),repo.toUtf8()));
    }
    inline Q_INVOKABLE PackageList getGroupPackages(const QString &group)
    {
        return PackageList(pamac_database_get_group_pkgs(db.get(),group.toUtf8()));
    }
    inline Q_INVOKABLE QStringList getIgnorePkgs()
    {
        QStringList result;
        auto list = pamac_database_get_ignorepkgs(db.get());
        for(auto el = list;el!=nullptr;el=el->next)
        {
            result.append(QString::fromUtf8(static_cast<char*>(el->data)));
        }

        g_list_free_full(list,g_free);

        return result;
    }

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);
    inline Q_INVOKABLE PackageList searchPkgs(const QString & name)
    {
        return PackageList(pamac_database_search_pkgs(db.get(),name.toUtf8()));
    }

    Q_INVOKABLE void getUpdatesAsync();

    inline operator PamacDatabase*(){return db.get();}

    Config config() const
    {
        return m_config;
    }

    bool getCheckspace() const
    {
       return pamac_database_get_checkspace(db.get());
    }

public slots:
    void setConfig(Config config)
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
    std::shared_ptr<PamacDatabase> db;
    Config m_config;
    bool m_checkspace;
};
} //namespace PamacQt
