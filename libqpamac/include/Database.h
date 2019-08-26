#pragma once

extern "C"{
#include "pamac.h"
}
#include <QObject>
#include <Package.h>
#include <AsyncHelpers.h>
#include <memory>
#include <Config.h>
#include <Updates.h>
#include <HistoryItemModel.h>


namespace LibQPamac {
class Database:public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config config READ config WRITE setConfig NOTIFY configChanged)

    Q_PROPERTY(bool checkspace READ getCheckspace CONSTANT)
    Q_PROPERTY(bool asynchronous MEMBER m_asynchronous)
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

    Q_INVOKABLE PackageDetails getPkgDetails(const QString &pkgname,const QString &app_name = "",bool useSyncDB = false);
    Q_INVOKABLE GenericQmlFuture getAurPkgDetails(const QString &pkgname);
    Q_INVOKABLE GenericQmlFuture cloneBuildFiles(const QString& pkgname,bool overwrite = true);


    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();


    Q_INVOKABLE QStringList getIgnorePkgs();
    Q_INVOKABLE GenericQmlFuture searchPkgsInAurAsync(const QString &name);

    Q_INVOKABLE GenericQmlFuture getCategoryPackagesAsync(const QString &category);
    Q_INVOKABLE GenericQmlFuture searchPkgsAsync(const QString &name);
    Q_INVOKABLE GenericQmlFuture getGroupPackagesAsync(const QString &group);
    Q_INVOKABLE GenericQmlFuture getRepoPackagesAsync(const QString &repo);
    Q_INVOKABLE GenericQmlFuture getInstalledAppsAsync();
    Q_INVOKABLE GenericQmlFuture getInstalledPackagesAsync(InstalledPackageTypes type);
    Q_INVOKABLE GenericQmlFuture getAurPackages(const QStringList &nameList);

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);

    Config config() const
    {
        return m_config;
    }
    PamacDatabase* getHandle(){
        return handle;
    }

    Q_INVOKABLE QStringList getMirrorsCountries();
    Q_INVOKABLE QString getMirrorsChoosenCountry();

    Q_INVOKABLE void getUpdatesAsync();

    Q_INVOKABLE Package getInstalledPackage(const QString& name);

    Q_INVOKABLE Package getSyncPackage(const QString& name);
    Q_INVOKABLE GenericQmlFuture getAurPackage(const QString& name);

    Q_INVOKABLE QVariantList findPackagesByName(const QStringList& names);

    bool getCheckspace();

    inline void refresh(){
        pamac_database_refresh(handle);
    }
    Q_INVOKABLE QList<QVariant> getHistory();



public Q_SLOTS:
    void setConfig(const Config& config);

Q_SIGNALS:
    void updatesReady(Updates upds);
    void getUpdatesProgress(uint percent);


    void checkspaceChanged(bool checkspace);

    void configChanged(Config config);


private:
    PamacDatabase* handle;
    void init();
    Config m_config;
    bool m_asynchronous;
};

} //namespace LibQPamac
