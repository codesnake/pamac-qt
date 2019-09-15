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
#include <AlpmPackage.h>
#include <AurPackage.h>


namespace LibQPamac {
class Database:public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config config READ config WRITE setConfig NOTIFY configChanged)

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

//    Q_INVOKABLE QFile cloneBuildFiles(const QString& pkgname,bool overwrite = true) const;


    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();


    Q_INVOKABLE QStringList getIgnorePkgs();
    Q_INVOKABLE QList<AurPackage> searchPkgsInAurAsync(const QString &name);

    Q_INVOKABLE QList<AlpmPackage> getCategoryPackagesAsync(const QString &category);
    Q_INVOKABLE QList<AlpmPackage> searchPkgs(const QString &name);
    Q_INVOKABLE QList<AlpmPackage> getGroupPackagesAsync(const QString &group);
    Q_INVOKABLE QList<AlpmPackage> getRepoPackagesAsync(const QString &repo);
    Q_INVOKABLE QList<AlpmPackage> getInstalledAppsAsync();
    Q_INVOKABLE QList<AlpmPackage> getInstalledPackagesAsync(InstalledPackageTypes type);
    Q_INVOKABLE QList<AurPackage> getAurPackages(const QStringList &nameList);

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

    Q_INVOKABLE Updates getUpdatesAsync();

    Q_INVOKABLE Package getInstalledPackage(const QString& name);

    Q_INVOKABLE Package getSyncPackage(const QString& name);
    Q_INVOKABLE AurPackage getAurPackage(const QString& name);

    Q_INVOKABLE QVariantList findPackagesByName(const QStringList& names);


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
