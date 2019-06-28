#pragma once

extern "C"{
#include "pamac.h"
}
#include <QObject>
#include <RepoPackage.h>
#include <PackageList.h>
#include <AsyncHelpers.h>
#include <memory>
#include <Config.h>
#include <Updates.h>
#include <HistoryItemModel.h>

#define PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(method)\
    [](GObject* parent,GAsyncResult* result,void* futurePtr){\
    auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);\
    if(future->isRunning()){\
    future->setFuture(QVariant::fromValue(RepoPackageList::fromGList(method(reinterpret_cast<PamacDatabase*>(parent),result))));\
    } else {\
    delete future;\
    }\
    }
#define PAMAC_QT_AUR_PACKAGELIST_ASYNC_CALLBACK(method)\
    [](GObject* parent,GAsyncResult* result,void* futurePtr){\
    auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);\
    if(future->isRunning()){\
    future->setFuture(QVariant::fromValue(AURPackageList::fromGList(method(reinterpret_cast<PamacDatabase*>(parent),result))));\
    } else {\
    delete future;\
    }\
    }
#define PAMAC_QT_AUR_PACKAGE_DETAILS_ASYNC_CALLBACK(method)\
    [](GObject* parent,GAsyncResult* result,void* futurePtr){\
    auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);\
    if(future->isRunning()){\
    future->setFuture(QVariant::fromValue(AURPackageDetails(method(reinterpret_cast<PamacDatabase*>(parent),result))));\
    } else {\
    delete future;\
    }\
    }
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

    Q_INVOKABLE RepoPackageDetails getPkgDetails(const QString &pkgname,const QString &app_name = "",bool useSyncDB = false);
    Q_INVOKABLE GenericQmlFuture getAurPkgDetails(const QString &pkgname);
    Q_INVOKABLE GenericQmlFuture cloneBuildFiles(const QString& pkgname,bool overwrite = true);

    Q_INVOKABLE RepoPackageList getInstalledApps();

    Q_INVOKABLE RepoPackageList getInstalledPackages(InstalledPackageTypes type);
    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();

    Q_INVOKABLE RepoPackageList getCategoryPackages(const QString &category);
    Q_INVOKABLE RepoPackageList getRepoPackages(const QString &repo);
    Q_INVOKABLE RepoPackageList getGroupPackages(const QString &group);
    Q_INVOKABLE QStringList getIgnorePkgs();
    Q_INVOKABLE GenericQmlFuture searchPkgsInAurAsync(const QString &name);

    Q_INVOKABLE GenericQmlFuture getCategoryPackagesAsync(const QString &category);
    Q_INVOKABLE GenericQmlFuture searchPkgsAsync(const QString &name);
    Q_INVOKABLE GenericQmlFuture getGroupPackagesAsync(const QString &group);
    Q_INVOKABLE GenericQmlFuture getRepoPackagesAsync(const QString &repo);
    Q_INVOKABLE GenericQmlFuture getInstalledAppsAsync();
    Q_INVOKABLE GenericQmlFuture getInstalledPackagesAsync(InstalledPackageTypes type);

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);
    Q_INVOKABLE RepoPackageList searchPkgs(const QString & name);

    Config config() const
    {
        return m_config;
    }

    Q_INVOKABLE QStringList getMirrorsCountries();
    Q_INVOKABLE QString getMirrorsChoosenCountry();

    Q_INVOKABLE void getUpdatesAsync();

    Q_INVOKABLE RepoPackageList getPending(const QStringList& toInstall, const QStringList& toRemove);

    Q_INVOKABLE RepoPackage getInstalledPackage(const QString& name);

    Q_INVOKABLE RepoPackage getSyncPackage(const QString& name);
    Q_INVOKABLE GenericQmlFuture getAurPackage(const QString& name);
    inline operator PamacDatabase*(){return m_db.get();}



    bool getCheckspace() const;

    inline void refresh(){
        pamac_database_refresh(m_db.get());
    }
    Q_INVOKABLE QList<HistoryItem> getHistory();



public Q_SLOTS:
    void setConfig(const Config& config);

Q_SIGNALS:
    void updatesReady(Updates upds);
    void getUpdatesProgress(uint percent);


    void checkspaceChanged(bool checkspace);

    void configChanged(Config config);


private:
    void init();
    std::shared_ptr<PamacDatabase> m_db;
    Config m_config;
    bool m_asynchronous;
};

} //namespace LibQPamac
