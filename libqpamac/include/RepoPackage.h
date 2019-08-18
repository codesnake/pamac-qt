#pragma once
#include "pamac.h"
#include <QObject>
#include <QUrl>
#include <QMetaType>
#include <memory>
#include "Utils.h"
#include <QtDebug>
namespace LibQPamac {
class RepoPackage
{
    Q_GADGET
public:

    RepoPackage()=default;

    RepoPackage(PamacPackage* package){
        m_handle=package;
        g_object_ref(m_handle);

    }
    RepoPackage(const RepoPackage& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
    }
    RepoPackage operator =(const RepoPackage& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
        return *this;
    }
    RepoPackage(void* packageData){
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


    ~RepoPackage(){
//        g_object_unref(m_handle);
    }

    operator PamacPackage*(){
        return this->m_handle;
    }
private:
    PamacPackage* m_handle;
};

class RepoPackageDetails
{
    Q_GADGET
public:

    RepoPackageDetails()= default;
    RepoPackageDetails(const RepoPackageDetails& another){
        m_handle=another.m_handle;
        g_object_ref(m_handle);
    }
    RepoPackageDetails operator =(const RepoPackageDetails& another){
        m_handle = another.m_handle;
        g_object_ref(m_handle);
        return *this;
    }
    RepoPackageDetails(PamacPackageDetails* details){
        m_handle = details;
        g_object_ref(m_handle);
    }
    ~RepoPackageDetails(){
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
Q_DECLARE_METATYPE(LibQPamac::RepoPackage)
Q_DECLARE_METATYPE(LibQPamac::RepoPackageDetails)
