#pragma once
#include <QObject>
#include <Package.h>
#include <Utils.h>
namespace LibQPamac {


class SnapPackage: public Package{
    Q_GADGET
    PAMAC_QT_STRING_PROPERTY_GET(channel, pamac_snap_package_get_channel(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(publisher, pamac_snap_package_get_publisher(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(license, pamac_snap_package_get_license(m_handle))
    PAMAC_QT_STRING_PROPERTY_GET(confined, pamac_snap_package_get_confined(m_handle))

public:
    SnapPackage() = default;
    SnapPackage(PamacSnapPackage* pkg):
        Package(&pkg->parent_instance),
        m_handle(pkg){}

    static SnapPackage fromData(void* data){return {reinterpret_cast<PamacSnapPackage*>(data)};};


    PamacSnapPackage* handle() const {return m_handle;}

protected:
    PamacSnapPackage *m_handle;
};

} // namespace LibQPamac
Q_DECLARE_METATYPE(LibQPamac::SnapPackage)
