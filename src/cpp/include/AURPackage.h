#pragma once
#include "pamac.h"
#include <QObject>
#include <memory>
#include "Utils.h"
namespace PamacQt {


class AURPackage
{
    Q_GADGET
private:
    PamacAURPackage* m_pkg;
public:
    AURPackage(PamacAURPackage* package):m_pkg(package){}
    AURPackage() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_get_name(m_pkg))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_get_version(m_pkg))
    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_aur_package_get_installed_version(m_pkg))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_get_desc(m_pkg))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_get_packagebase(m_pkg))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_get_outofdate(m_pkg))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_get_popularity(m_pkg))

};

class AURPackageDetails
{
    Q_GADGET
private:
    std::shared_ptr<PamacAURPackageDetails> m_details;
public:

    AURPackageDetails(PamacAURPackageDetails* details):m_details(std::shared_ptr<PamacAURPackageDetails>(details,g_object_unref)){}
    AURPackageDetails() = default;

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_aur_package_details_get_name(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_aur_package_details_get_version(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_aur_package_details_get_desc(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(packageBase,pamac_aur_package_details_get_packagebase(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(url,pamac_aur_package_details_get_url(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(maintainer,pamac_aur_package_details_get_maintainer(m_details.get()))
    PAMAC_QT_DATETIME_PROPERTY_GET(firstSubmitted,pamac_aur_package_details_get_firstsubmitted(m_details.get()))
    PAMAC_QT_DATETIME_PROPERTY_GET(lastModified,pamac_aur_package_details_get_lastmodified(m_details.get()))
    PAMAC_QT_DATETIME_PROPERTY_GET(outOfDate,pamac_aur_package_details_get_outofdate(m_details.get()))
    PAMAC_QT_DOUBLE_PROPERTY_GET(popularity,pamac_aur_package_details_get_popularity(m_details.get()))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses, pamac_aur_package_details_get_licenses(m_details.get()))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends, pamac_aur_package_details_get_depends(m_details.get()))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends, pamac_aur_package_details_get_optdepends(m_details.get()))

};

} // namespace PamacQt

Q_DECLARE_METATYPE(PamacQt::AURPackage)
Q_DECLARE_METATYPE(PamacQt::AURPackageDetails)
