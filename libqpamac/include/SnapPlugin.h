#pragma once
#include <pamac.h>
#include <QObject>
#include <SnapPackage.h>

namespace LibQPamac {

class SnapPlugin: public QObject {
    Q_OBJECT
public:
    explicit SnapPlugin(QObject* parent = nullptr);

    Q_INVOKABLE QList<SnapPackage> searchSnaps(const QString&  searchString ){
        return Utils::gListToQList<SnapPackage>(pamac_snap_plugin_search_snaps(m_handle,searchString.toUtf8()),SnapPackage::fromData);
    }
    Q_INVOKABLE bool isInstalledSnap(const QString& name){
        return pamac_snap_plugin_is_installed_snap(m_handle, name.toUtf8());
    }
    Q_INVOKABLE QList<SnapPackage> getInstalledSnaps(){
        return Utils::gListToQList<SnapPackage>(pamac_snap_plugin_get_installed_snaps(m_handle),SnapPackage::fromData);
    }
    Q_INVOKABLE QList<SnapPackage> getCategorySnaps(const QString& category){
        return Utils::gListToQList<SnapPackage>(pamac_snap_plugin_get_category_snaps(m_handle,category.toUtf8()),SnapPackage::fromData);
    }

protected:
    PamacSnapPlugin* m_handle;
};

} // namespace LibQPamac
