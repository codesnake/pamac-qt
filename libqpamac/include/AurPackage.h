#pragma once
#include "pamac.h"
#include "Utils.h"
#include <memory>
#include <AlpmPackage.h>
namespace LibQPamac {
class AurPackage : public AlpmPackage
{
    Q_GADGET
public:
    AurPackage(PamacAURPackage* package):
        m_handle(package),
        AlpmPackage(&package->parent_instance){}
    AurPackage(void* packageData):
    m_handle(reinterpret_cast<PamacAURPackage*>(packageData)),
    AlpmPackage(&reinterpret_cast<PamacAURPackage*>(packageData)->parent_instance){
    }
    AurPackage() = default;

    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_get_packagebase(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_get_outofdate(m_handle))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_get_popularity(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(maintainer,pamac_aur_package_get_maintainer(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(firstSubmitted,pamac_aur_package_get_firstsubmitted(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(lastModified,pamac_aur_package_get_lastmodified(m_handle))

    private:
        PamacAURPackage* m_handle;
};

} // namespace LibQPamac

Q_DECLARE_METATYPE(LibQPamac::AurPackage)
