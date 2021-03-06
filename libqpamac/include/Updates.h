#pragma once
#include "pamac.h"
#include <QObject>
#include "Utils.h"
#include "Package.h"
#include "AurPackage.h"
#include "AlpmPackage.h"
namespace LibQPamac {
class Updates
{
    Q_GADGET
public:
    Updates()=default;
    Updates(PamacUpdates* upd):m_updates(std::shared_ptr<PamacUpdates>(upd,g_object_unref)){}
    inline Q_INVOKABLE QVariantList getReposUpdates(){
        return Utils::gSListToQList<QVariant>(pamac_updates_get_repos_updates(m_updates.get()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AlpmPackage));
    }
    inline Q_INVOKABLE QVariantList getAurUpdates(){
        return Utils::gSListToQList<QVariant>(pamac_updates_get_aur_updates(m_updates.get()),PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(AurPackage));
    }
private:
    std::shared_ptr<PamacUpdates> m_updates;
};
} //namespace LibQPamac
