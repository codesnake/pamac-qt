#pragma once
#include <QObject>
#include <Package.h>
#include <memory>
#include <Config.h>
#include <Updates.h>
#include <HistoryItemModel.h>
#include <AlpmPackage.h>
#include <AurPackage.h>
#include <pamac.h>

namespace LibQPamac {
class Database:public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config* config READ config WRITE setConfig NOTIFY configChanged)

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

    Q_INVOKABLE QVariantList searchPkgsInAur(const QString &name);

    Q_INVOKABLE QVariantList getCategoryPackages(const QString &category);
    Q_INVOKABLE QVariantList searchPkgs(const QString &name);
    Q_INVOKABLE QVariantList getGroupPackages(const QString &group);
    Q_INVOKABLE QVariantList getRepoPackages(const QString &repo);
    Q_INVOKABLE QVariantList getInstalledApps();
    Q_INVOKABLE QVariantList getInstalledPackages(InstalledPackageTypes type);
    Q_INVOKABLE QVariantList getAurPackages(const QStringList &nameList);
    Q_INVOKABLE AlpmPackage getPkg(const QString& name);

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);

    Config* config()
    {
        return m_config;
    }
    PamacDatabase* getHandle(){
        return handle;
    }

    Q_INVOKABLE QStringList getMirrorsCountries();
    Q_INVOKABLE QString getMirrorsChoosenCountry();

    Q_INVOKABLE Updates getUpdates();

    Q_INVOKABLE AlpmPackage getInstalledPackage(const QString& name);

    Q_INVOKABLE AlpmPackage getSyncPackage(const QString& name);
    Q_INVOKABLE AurPackage getAurPackage(const QString& name);

    Q_INVOKABLE QVariantList findPackagesByName(const QStringList& names);


    inline void refresh(){
        pamac_database_refresh(handle);
    }

    Q_INVOKABLE QList<QVariant> getHistory();



public Q_SLOTS:
    void setConfig(Config *config);

Q_SIGNALS:
    void updatesReady(Updates upds);
    void getUpdatesProgress(uint percent);



    void configChanged(Config* config);


private:
    PamacDatabase* handle;
    void init();
    Config* m_config = nullptr;
    bool m_asynchronous;
};

} //namespace LibQPamac
