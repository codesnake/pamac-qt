#pragma once
#include "pamac.h"
#include <QObject>
#include <QUrl>
#include <QMetaType>
#include <memory>
#include "Utils.h"
namespace PamacQt {
class RepoPackage
{
    Q_GADGET
private:
    PamacPackage* m_package;

public:

    RepoPackage()=default;

    RepoPackage(PamacPackage* package):m_package(package){}

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_get_name(m_package))

    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_get_app_name(m_package))

    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_package_get_version(m_package))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_get_installed_version(m_package))

    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_get_desc(m_package))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_get_icon(m_package))

    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_get_repo(m_package))

    PAMAC_QT_INT_PROPERTY_GET(size,pamac_package_get_installed_size(m_package))

    PAMAC_QT_INT_PROPERTY_GET(downloadSize,pamac_package_get_download_size(m_package))


    operator PamacPackage*()
    {
        return m_package;
    }

};

class RepoPackageDetails
{
    Q_GADGET
public:

    RepoPackageDetails():m_details(nullptr){}

    RepoPackageDetails(PamacPackageDetails* details):m_details(std::shared_ptr<PamacPackageDetails>(details,g_object_unref)){}

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_details_get_name(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_details_get_app_name(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_details_get_desc(m_details.get()))

    PAMAC_QT_STRING_PROPERTY_GET(longDesc,pamac_package_details_get_long_desc(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_details_get_repo(m_details.get()))
    PAMAC_QT_STRING_PROPERTY_GET(packager,pamac_package_details_get_packager(m_details.get()))

    PAMAC_QT_URL_PROPERTY_GET(url,pamac_package_details_get_url(m_details.get()))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_details_get_icon(m_details.get()))

    PAMAC_QT_URL_PROPERTY_GET(screenshotUrl,pamac_package_details_get_screenshot(m_details.get()))

    PAMAC_QT_DATETIME_PROPERTY_GET(installDate,pamac_package_details_get_installdate(m_details.get()))

    PAMAC_QT_DATETIME_PROPERTY_GET(buildDate,pamac_package_details_get_builddate(m_details.get()))

    PAMAC_QT_STRING_PROPERTY_GET(installReason,pamac_package_details_get_reason(m_details.get()))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_details_get_installed_version(m_details.get()))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends,pamac_package_details_get_depends(m_details.get()))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends,pamac_package_details_get_optdepends(m_details.get()))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses,pamac_package_details_get_licenses(m_details.get()))

private:
    std::shared_ptr<PamacPackageDetails> m_details;
};
} //namespace PamacQt
Q_DECLARE_METATYPE(PamacQt::RepoPackage)
Q_DECLARE_METATYPE(PamacQt::RepoPackageDetails)
