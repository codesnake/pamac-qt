#pragma once
#include "pamac.h"
#include <QObject>
#include "Utils.h"
#include "RepoPackage.h"
#include "AURPackage.h"
#include "PackageList.h"
namespace PamacQt {
class Updates
{
    Q_GADGET
public:
    Updates()=default;
    Updates(PamacUpdates* upd):m_updates(std::shared_ptr<PamacUpdates>(upd,g_object_unref)){}
    inline Q_INVOKABLE RepoPackageList getReposUpdates(){
        return RepoPackageList::fromGList(pamac_updates_get_repos_updates(m_updates.get()));
    }
    inline Q_INVOKABLE AURPackageList getAurUpdates(){
        return AURPackageList::fromGList(pamac_updates_get_aur_updates(m_updates.get()));
    }
private:
    std::shared_ptr<PamacUpdates> m_updates;
};
} //namespace PamacQt
