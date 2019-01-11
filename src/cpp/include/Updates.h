#pragma once
#include "pamac.h"
#include <QObject>
#include "PackageList.h"
namespace PamacQt {
class Updates
{
    Q_GADGET
public:
    Updates(){}
    Updates(PamacUpdates* upd):m_updates(std::shared_ptr<PamacUpdates>(upd,g_object_unref)){}
    inline Q_INVOKABLE PackageList getReposUpdates(){
        return PackageList(pamac_updates_get_repos_updates(m_updates.get()));
    }
    inline Q_INVOKABLE PackageList getAurUpdates(){
        return PackageList(pamac_updates_get_aur_updates(m_updates.get()));
    }
private:
    std::shared_ptr<PamacUpdates> m_updates;
};
} //namespace PamacQt
