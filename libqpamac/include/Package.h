#pragma once
#include "pamac.h"
#include <QObject>
#include <QUrl>
#include <QMetaType>
#include <memory>
#include "Utils.h"
#include <QtDebug>
namespace LibQPamac {
class Package
{
    Q_GADGET
public:

    Package()=default;

    Package(PamacPackage* package){
        m_handle=package;
        g_object_ref(m_handle);

    }
    Package(const Package& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
    }
    Package operator =(const Package& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
        return *this;
    }
    Package(void* packageData){
        m_handle=reinterpret_cast<PamacPackage*>(packageData);
        g_object_ref(m_handle);
    }

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_get_name(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_get_app_name(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_package_get_version(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_get_installed_version(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_get_desc(m_handle))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_get_icon(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_get_repo(m_handle))

    PAMAC_QT_INT_PROPERTY_GET(size,pamac_package_get_installed_size(m_handle))

    PAMAC_QT_INT_PROPERTY_GET(downloadSize,pamac_package_get_download_size(m_handle))


    ~Package(){
//        g_object_unref(m_handle);
    }

    operator PamacPackage*(){
        return this->m_handle;
    }
private:
    PamacPackage* m_handle;
};

class PackageDetails
{
    Q_GADGET
public:

    PackageDetails()= default;
    PackageDetails(const PackageDetails& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
    }
    PackageDetails operator =(const PackageDetails& another){
        m_handle = another.m_handle;
        g_object_ref(m_handle);
        return *this;
    }
    PackageDetails(PamacPackageDetails* details){
        m_handle = details;
        g_object_ref(m_handle);
    }
    ~PackageDetails(){
        g_object_unref(m_handle);
    }
    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_details_get_name(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_details_get_app_name(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_details_get_desc(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(longDesc,pamac_package_details_get_long_desc(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_details_get_repo(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(packager,pamac_package_details_get_packager(m_handle))

    PAMAC_QT_URL_PROPERTY_GET(url,pamac_package_details_get_url(m_handle))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_details_get_icon(m_handle))

    PAMAC_QT_URL_PROPERTY_GET(screenshotUrl,pamac_package_details_get_screenshot(m_handle))

    PAMAC_QT_DATETIME_PROPERTY_GET(installDate,pamac_package_details_get_installdate(m_handle))

    PAMAC_QT_DATETIME_PROPERTY_GET(buildDate,pamac_package_details_get_builddate(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(installReason,pamac_package_details_get_reason(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_details_get_installed_version(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends,pamac_package_details_get_depends(m_handle))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends,pamac_package_details_get_optdepends(m_handle))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses,pamac_package_details_get_licenses(m_handle))

private:
    PamacPackageDetails* m_handle;
};
} //namespace LibQPamac
Q_DECLARE_METATYPE(LibQPamac::Package)
Q_DECLARE_METATYPE(LibQPamac::PackageDetails)
