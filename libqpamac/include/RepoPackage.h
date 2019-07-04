#pragma once
#include "pamac.h"
#include <QObject>
#include <QUrl>
#include <QMetaType>
#include <memory>
#include "Utils.h"
#include <QtDebug>
namespace LibQPamac {
class RepoPackage:public PamacPackage
{
    Q_GADGET
public:

    RepoPackage()=default;

    RepoPackage(PamacPackage* package){
        *static_cast<PamacPackage*>(this)=*package;
        g_object_ref(static_cast<PamacPackage*>(this));

    }
    RepoPackage(const RepoPackage& another){
        *static_cast<PamacPackage*>(this) = *static_cast<const PamacPackage*>(&another);
        g_object_ref(static_cast<PamacPackage*>(this));
    }
    RepoPackage operator =(const RepoPackage& another){
        *static_cast<PamacPackage*>(this) = *static_cast<const PamacPackage*>(&another);
        g_object_ref(static_cast<PamacPackage*>(this));
        return this;
    }

    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_get_name(this))

    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_get_app_name(this))

    PAMAC_QT_STRING_PROPERTY_GET(version,pamac_package_get_version(this))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_get_installed_version(this))

    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_get_desc(this))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_get_icon(this))

    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_get_repo(this))

    PAMAC_QT_INT_PROPERTY_GET(size,pamac_package_get_installed_size(this))

    PAMAC_QT_INT_PROPERTY_GET(downloadSize,pamac_package_get_download_size(this))


    ~RepoPackage(){
        g_object_unref(static_cast<PamacPackage*>(this));
    }

};

class RepoPackageDetails:public PamacPackageDetails
{
    Q_GADGET
public:

    RepoPackageDetails()= default;
    RepoPackageDetails(const RepoPackageDetails& another){
        *static_cast<PamacPackageDetails*>(this) = *static_cast<const PamacPackageDetails*>(&another);
        g_object_ref(this);
    }
    RepoPackageDetails operator =(const RepoPackageDetails& another){
        *static_cast<PamacPackageDetails*>(this) = *static_cast<const PamacPackageDetails*>(&another);
        g_object_ref(this);
        return this;
    }
    RepoPackageDetails(PamacPackageDetails* details){
        *static_cast<PamacPackageDetails*>(this)=*details;
        g_object_ref(this);
    }
    ~RepoPackageDetails(){
        g_object_unref(this);
    }
    PAMAC_QT_STRING_PROPERTY_GET(name,pamac_package_details_get_name(this))
    PAMAC_QT_STRING_PROPERTY_GET(appName,pamac_package_details_get_app_name(this))
    PAMAC_QT_STRING_PROPERTY_GET(desc,pamac_package_details_get_desc(this))

    PAMAC_QT_STRING_PROPERTY_GET(longDesc,pamac_package_details_get_long_desc(this))
    PAMAC_QT_STRING_PROPERTY_GET(repo,pamac_package_details_get_repo(this))
    PAMAC_QT_STRING_PROPERTY_GET(packager,pamac_package_details_get_packager(this))

    PAMAC_QT_URL_PROPERTY_GET(url,pamac_package_details_get_url(this))

    PAMAC_QT_URL_PROPERTY_GET(iconUrl,pamac_package_details_get_icon(this))

    PAMAC_QT_URL_PROPERTY_GET(screenshotUrl,pamac_package_details_get_screenshot(this))

    PAMAC_QT_DATETIME_PROPERTY_GET(installDate,pamac_package_details_get_installdate(this))

    PAMAC_QT_DATETIME_PROPERTY_GET(buildDate,pamac_package_details_get_builddate(this))

    PAMAC_QT_STRING_PROPERTY_GET(installReason,pamac_package_details_get_reason(this))

    PAMAC_QT_STRING_PROPERTY_GET(installedVersion,pamac_package_details_get_installed_version(this))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends,pamac_package_details_get_depends(this))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends,pamac_package_details_get_optdepends(this))
    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses,pamac_package_details_get_licenses(this))

};
} //namespace LibQPamac
Q_DECLARE_METATYPE(LibQPamac::RepoPackage)
Q_DECLARE_METATYPE(LibQPamac::RepoPackageDetails)
