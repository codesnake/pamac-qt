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
    Updates(PamacUpdates* upd):upd(std::shared_ptr<PamacUpdates>(upd,[](PamacUpdates *ptr){ g_object_unref(ptr);})){}
    inline Q_INVOKABLE PackageList getReposUpdates(){
        return PackageList(pamac_updates_get_repos_updates(upd.get()));
    }
    inline Q_INVOKABLE PackageList getAurUpdates(){
        return PackageList(pamac_updates_get_aur_updates(upd.get()));
    }
private:
    std::shared_ptr<PamacUpdates> upd;
};
} //namespace PamacQt
