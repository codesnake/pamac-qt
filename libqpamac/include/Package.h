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
    }

    static Package fromData(void* data){
        return Package(reinterpret_cast<PamacPackage*>(data));
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

    PAMAC_QT_STRING_PROPERTY_GET(launchable,pamac_package_get_launchable(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(longDesc,pamac_package_get_long_desc(m_handle))

    PAMAC_QT_URL_PROPERTY_GET(url,pamac_package_get_url(m_handle))

    PAMAC_QT_DATETIME_PROPERTY_GET(installDate,pamac_package_get_installdate(m_handle))


    PamacPackage* handle(){return m_handle;}

protected:
    PamacPackage* m_handle;
};

} //namespace LibQPamac
Q_DECLARE_METATYPE(LibQPamac::Package)
