#pragma once
#include "pamac.h"
#include "Utils.h"
#include <memory>

namespace LibQPamac {
class AURPackage: public PamacAURPackage
{
    Q_GADGET
public:
    AURPackage(PamacAURPackage* package){*static_cast<PamacAURPackage*>(this) = *package;}
    AURPackage() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_get_name(this))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_get_version(this))
    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_aur_package_get_installed_version(this))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_get_desc(this))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_get_packagebase(this))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_get_outofdate(this))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_get_popularity(this))

};

class AURPackageDetails: public PamacAURPackageDetails
{
    Q_GADGET
private:
public:

    AURPackageDetails(PamacAURPackageDetails* details){
        *static_cast<PamacAURPackageDetails*>(this) = *details;
    }
    AURPackageDetails() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_details_get_name(this))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_details_get_version(this))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_details_get_desc(this))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_details_get_packagebase(this))
    PAMAC_QT_STRING_PROPERTY_GET(url,pamac_aur_package_details_get_url(this))
    PAMAC_QT_STRING_PROPERTY_GET(maintainer,pamac_aur_package_details_get_maintainer(this))
    PAMAC_QT_DATETIME_PROPERTY_GET(firstSubmitted,pamac_aur_package_details_get_firstsubmitted(this))
    PAMAC_QT_DATETIME_PROPERTY_GET(lastModified,pamac_aur_package_details_get_lastmodified(this))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_details_get_outofdate(this))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_details_get_popularity(this))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses, pamac_aur_package_details_get_licenses(this))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends, pamac_aur_package_details_get_depends(this))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends, pamac_aur_package_details_get_optdepends(this))

};

} // namespace LibQPamac

Q_DECLARE_METATYPE(LibQPamac::AURPackage)
Q_DECLARE_METATYPE(LibQPamac::AURPackageDetails)
