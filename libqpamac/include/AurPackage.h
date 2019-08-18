#pragma once
#include "pamac.h"
#include "Utils.h"
#include <memory>

namespace LibQPamac {
class AURPackage
{
    Q_GADGET
public:
    AURPackage(PamacAURPackage* package){m_handle = package;}
    AURPackage(void* packageData){
        m_handle = reinterpret_cast<PamacAURPackage*>(packageData);
    }
    AURPackage() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_get_name(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_get_version(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_aur_package_get_installed_version(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_get_desc(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_get_packagebase(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_get_outofdate(m_handle))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_get_popularity(m_handle))

    private:
        PamacAURPackage* m_handle;
};

class AURPackageDetails: public PamacAURPackageDetails
{
    Q_GADGET
private:
public:

    AURPackageDetails(PamacAURPackageDetails* details){
        m_handle = details;
    }
    AURPackageDetails() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_details_get_name(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_details_get_version(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_details_get_desc(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_details_get_packagebase(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(url,pamac_aur_package_details_get_url(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(maintainer,pamac_aur_package_details_get_maintainer(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(firstSubmitted,pamac_aur_package_details_get_firstsubmitted(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(lastModified,pamac_aur_package_details_get_lastmodified(m_handle))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_details_get_outofdate(m_handle))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_details_get_popularity(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses, pamac_aur_package_details_get_licenses(m_handle))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends, pamac_aur_package_details_get_depends(m_handle))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends, pamac_aur_package_details_get_optdepends(m_handle))

    private:
        PamacAURPackageDetails* m_handle;
};

} // namespace LibQPamac

Q_DECLARE_METATYPE(LibQPamac::AURPackage)
Q_DECLARE_METATYPE(LibQPamac::AURPackageDetails)
