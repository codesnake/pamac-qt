#pragma once
#include "pamac.h"
#include <QObject>
#include "Utils.h"
#include "RepoPackage.h"
#include "AurPackage.h"
namespace LibQPamac {
class Updates
{
    Q_GADGET
public:
    Updates()=default;
    Updates(PamacUpdates* upd):m_updates(std::shared_ptr<PamacUpdates>(upd,g_object_unref)){}
    inline Q_INVOKABLE QVariantList getReposUpdates(){
        return Utils::gListToQList<QVariant>(pamac_updates_get_repos_updates(m_updates.get()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(RepoPackage));
    }
    inline Q_INVOKABLE QVariantList getAurUpdates(){
        return Utils::gListToQList<QVariant>(pamac_updates_get_aur_updates(m_updates.get()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AURPackage));
    }
private:
    std::shared_ptr<PamacUpdates> m_updates;
};
} //namespace LibQPamac
