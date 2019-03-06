#pragma once
#include "pamac.h"
#include <QObject>
#include <RepoPackage.h>
#include <QtDebug>
#include <iostream>
#include <QJSValue>
#include <QJSEngine>
#include <QFile>
#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>
#include "Updates.h"
#include "Config.h"
#include "Utils.h"
#include "AsyncHelpers.h"
#include "AURPackage.h"

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
namespace PamacQt {
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

    inline Q_INVOKABLE RepoPackageDetails getPkgDetails(const QString &pkgname,const QString &app_name = "",bool useSyncDB = false)
    {
        return RepoPackageDetails(pamac_database_get_pkg_details(m_db.get(),pkgname.toUtf8(),app_name.toUtf8(),useSyncDB));
    }
    inline Q_INVOKABLE QmlFuture getAURPkgDetails(const QString &pkgname)
    {
        auto future = new QmlFutureImpl;
        pamac_database_get_aur_pkg_details(m_db.get(),pkgname.toUtf8(),PAMAC_QT_AUR_PACKAGE_DETAILS_ASYNC_CALLBACK(pamac_database_get_aur_pkg_details_finish),future);
        return QmlFuture(future);

    }
inline Q_INVOKABLE QmlFuture cloneBuildFiles(const QString& pkgname,bool overwrite = true){
    auto future = new QmlFutureImpl;
    pamac_database_clone_build_files(m_db.get(),pkgname.toUtf8(),overwrite,
                                     [](GObject* parent,GAsyncResult* result,void* futurePtr){
        auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);

        auto folder = pamac_database_clone_build_files_finish(reinterpret_cast<PamacDatabase*>(parent),result);

        if(future->isRunning()){
            future->setFuture(QVariant(QString::fromUtf8(g_file_get_path(folder))));
        }   else {
            delete future;
        }

    }
        ,future);
    return QmlFuture(future);
}

    inline Q_INVOKABLE RepoPackageList getInstalledApps()
    {
        return RepoPackageList::fromGList(pamac_database_get_installed_apps(m_db.get()));
    }

    Q_INVOKABLE RepoPackageList getInstalledPackages(InstalledPackageTypes type);
    Q_INVOKABLE QStringList getRepos();
    Q_INVOKABLE QStringList getGroups();

    inline Q_INVOKABLE RepoPackageList getCategoryPackages(const QString &category)
    {
        return RepoPackageList::fromGList(pamac_database_get_category_pkgs(m_db.get(),category.toUtf8()));
    }
    inline Q_INVOKABLE RepoPackageList getRepoPackages(const QString &repo)
    {
        return RepoPackageList::fromGList(pamac_database_get_repo_pkgs(m_db.get(),repo.toUtf8()));
    }
    inline Q_INVOKABLE RepoPackageList getGroupPackages(const QString &group)
    {
        return RepoPackageList::fromGList(pamac_database_get_group_pkgs(m_db.get(),group.toUtf8()));
    }
    inline Q_INVOKABLE QStringList getIgnorePkgs()
    {
        return Utils::gListToQStringList(pamac_database_get_ignorepkgs(m_db.get()),true);
    }
    inline Q_INVOKABLE QmlFuture searchPkgsInAurAsync(const QString &name)
    {

        auto future = new QmlFutureImpl;

        pamac_database_search_in_aur(m_db.get(),name.toUtf8(),PAMAC_QT_AUR_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_in_aur_finish),future);
        return QmlFuture(future);
    }

    inline Q_INVOKABLE QmlFuture getCategoryPackagesAsync(const QString &category)
    {
        auto future = new QmlFutureImpl;

        pamac_database_get_category_pkgs_async(m_db.get(),category.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_category_pkgs_finish),future);
        return QmlFuture(future);
    }
    inline Q_INVOKABLE QmlFuture searchPkgsAsync(const QString &name)
    {
        auto future = new QmlFutureImpl;

        pamac_database_search_pkgs_async(m_db.get(),name.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_search_pkgs_finish),future);
        return QmlFuture(future);
    }
    inline Q_INVOKABLE QmlFuture getGroupPackagesAsync(const QString &group)
    {
        auto future = new QmlFutureImpl;

        pamac_database_get_group_pkgs_async(m_db.get(),group.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_group_pkgs_finish),future);
        return QmlFuture(future);
    }
    inline Q_INVOKABLE QmlFuture getRepoPackagesAsync(const QString &repo)
    {
        auto future = new QmlFutureImpl;

        pamac_database_get_repo_pkgs_async(m_db.get(),repo.toUtf8(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_repo_pkgs_finish),future);
        return QmlFuture(future);
    }
    inline Q_INVOKABLE QmlFuture getInstalledAppsAsync(){
        auto future = new QmlFutureImpl;
        pamac_database_get_installed_apps_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_installed_apps_finish),future);
        return QmlFuture(future);
    }
    inline Q_INVOKABLE QmlFuture getInstalledPackagesAsync(InstalledPackageTypes type){
        auto future = new QmlFutureImpl;
        switch (type) {
        case Installed:
            pamac_database_get_installed_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_installed_pkgs_finish),future);
            break;
        case Explicitly:
            pamac_database_get_explicitly_installed_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_explicitly_installed_pkgs_finish),future);
            break;
        case Orphans:
            pamac_database_get_orphans_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_orphans_finish),future);
            break;
        case Foreign:
            pamac_database_get_foreign_pkgs_async(m_db.get(),PAMAC_QT_REPO_PACKAGELIST_ASYNC_CALLBACK(pamac_database_get_foreign_pkgs_finish),future);
            break;
        }

        return QmlFuture(future);
    }

    Q_INVOKABLE QStringList getPkgFiles(const QString &name);
    inline Q_INVOKABLE RepoPackageList searchPkgs(const QString & name)
    {
        return RepoPackageList::fromGList(pamac_database_search_pkgs(m_db.get(),name.toUtf8()));
    }

    Config config() const
    {
        return m_config;
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

    Q_INVOKABLE RepoPackageList getPending(const QStringList& toInstall, const QStringList& toRemove);
//    Q_INVOKABLE AURPackageList getPending(const QStringList& toBuild);

    Q_INVOKABLE RepoPackage getInstalledPackage(const QString& name){
        return RepoPackage(pamac_database_get_installed_pkg(m_db.get(),name.toUtf8()));
    }

    Q_INVOKABLE RepoPackage getSyncPackage(const QString& name){
        return RepoPackage(pamac_database_get_sync_pkg(m_db.get(),name.toUtf8()));
    }
Q_INVOKABLE QmlFuture getAurPackage(const QString& name){
        auto future = new QmlFutureImpl;
        pamac_database_get_aur_pkg(m_db.get(),name.toUtf8(),[](GObject* object,GAsyncResult* result,gpointer futurePtr){
            auto future = reinterpret_cast<QmlFutureImpl*>(futurePtr);
            if(future->isRunning()) {
                future->setFuture(QVariant::fromValue(AURPackage(pamac_database_get_aur_pkg_finish(reinterpret_cast<PamacDatabase*>(object),result))));
            } else {
                delete future;
}
        },future);

        return QmlFuture(future);
    }
    inline operator PamacDatabase*(){return m_db.get();}



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
    bool m_asynchronous;
    QThreadPool *pool;
};
} //namespace PamacQt
