#pragma once
#include <pamac.h>
#include <Package.h>
#include <Utils.h>
namespace LibQPamac {

class AlpmPackage : public Package
{
    Q_GADGET
public:
    AlpmPackage(PamacAlpmPackage* package){
        m_handle=package;
        Package::m_handle = &m_handle->parent_instance;

        g_object_ref(m_handle);

    }
    AlpmPackage(const AlpmPackage& another){
        m_handle=another.m_handle;
        Package::m_handle = &m_handle->parent_instance;
        g_object_ref(m_handle);
    }
    AlpmPackage operator =(const AlpmPackage& another){
        m_handle=another.m_handle;
        Package::m_handle = &m_handle->parent_instance;
        g_object_ref(m_handle);
        return *this;
    }
    AlpmPackage(void* packageData){
        m_handle=reinterpret_cast<PamacAlpmPackage*>(packageData);
        Package::m_handle = &m_handle->parent_instance;
        g_object_ref(m_handle);
    }
AlpmPackage() = default;

    PAMAC_QT_STRING_PROPERTY_GET(packager,pamac_alpm_package_get_packager(m_handle))

    PAMAC_QT_DATETIME_PROPERTY_GET(buildDate,pamac_alpm_package_get_builddate(m_handle))

    PAMAC_QT_STRING_PROPERTY_GET(installReason,pamac_alpm_package_get_reason(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(screenshots,pamac_alpm_package_get_screenshots(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(depends,pamac_alpm_package_get_depends(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(optDepends,pamac_alpm_package_get_optdepends(m_handle))

    PAMAC_QT_STRINGLIST_PROPERTY_GET(licenses,pamac_alpm_package_get_licenses(m_handle))

protected:
    PamacAlpmPackage* m_handle;
};

} // namespace LibQPamac
Q_DECLARE_METATYPE(LibQPamac::AlpmPackage)
