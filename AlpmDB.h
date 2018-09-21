#ifndef ALPMDB_H
#define ALPMDB_H
#include "pamac.h"
#include <QObject>
#include <AlpmPkg.h>
#include <AlpmList.h>
#include <QtDebug>

class AlpmDB:public QObject
{
    Q_OBJECT
public:
    AlpmDB(PamacDatabase* db,QObject* parent = nullptr);
    AlpmDB(QString configFile,QObject* parent = nullptr);

    Q_INVOKABLE QVariant getPkgDetails(const QString &pkgname,const QString &app_name = "")
    {
        auto det =
                pamac_database_get_pkg_details(db,pkgname.toUtf8(),app_name.toUtf8());

        return QVariant::fromValue(AlpmPkgDetails(det));
    }

    Q_INVOKABLE AlpmPackageList getInstalledApps()
    {
        auto list = pamac_database_get_installed_apps(db);
        return AlpmPackageList(list);
    }
    Q_INVOKABLE AlpmPackageList getInstalled()
    {
        auto list = pamac_database_get_installed_pkgs(db);
        return AlpmPackageList(list);
    }
    Q_INVOKABLE QStringList getRepos()
    {
        QStringList result;
        auto list = pamac_database_get_repos_names(db);

        for(auto el = list;el!=nullptr;el=el->next)
        {
            result.append(QString::fromUtf8(static_cast<char*>(el->data)));
        }

        return result;
    }
    Q_INVOKABLE AlpmPackageList getCategoryPackages(const QString &category)
    {
        auto list = pamac_database_get_category_pkgs(db,category.toUtf8());
        return AlpmPackageList(list);
    }
    Q_INVOKABLE AlpmPackageList getRepoPackages(const QString &repo)
    {
        auto list = pamac_database_get_repo_pkgs(db,repo.toUtf8());
        return AlpmPackageList(list);
    }

    Q_INVOKABLE QStringList getPkgFiles(const QString &name){
        QStringList result;
        auto list = pamac_database_get_pkg_files(db,name.toUtf8());
        for(auto el = list;el!=nullptr;el=el->next)
        {
            result.append(QString::fromUtf8(static_cast<char*>(el->data)));
        }
        return result;
    }

    operator PamacDatabase*(){return db;}

private:
    PamacDatabase* db;
};

#endif // ALPMDB_H
