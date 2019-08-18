#pragma once
#include "pamac.h"
#include <QObject>
#include <QtDebug>
#include <memory>
#include "Utils.h"

namespace LibQPamac{

class Config
{
    Q_GADGET
public:
    Config(PamacConfig* cfg)
    {
        m_handle = cfg;
    }
    Config(const QString& str)
    {
        m_handle = pamac_config_new(str.toUtf8());
    }
    Config()=default;

    PAMAC_QT_STRING_PROPERTY_GET(confPath,pamac_config_get_conf_path(m_handle))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(recurse,pamac_config_get_recurse(m_handle),
                                   setRecurse,pamac_config_set_recurse(m_handle,recurse))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(noUpdateHideIcon,pamac_config_get_no_update_hide_icon(m_handle),
                                   setNoUpdateHideIcon,pamac_config_set_no_update_hide_icon(m_handle,noUpdateHideIcon))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(enableAur,pamac_config_get_enable_aur(m_handle),
                                   setEnableAur,pamac_config_set_enable_aur(m_handle,enableAur))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(checkAurUpdates,pamac_config_get_check_aur_updates(m_handle),
                                   setCheckAurUpdates,pamac_config_set_check_aur_updates(m_handle,checkAurUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(downloadUpdates,pamac_config_get_download_updates(m_handle),
                                   setDownloadUpdates,pamac_config_set_download_updates(m_handle,downloadUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(cleanRmOnlyUninstalled,pamac_config_get_clean_rm_only_uninstalled(m_handle),
                                   setCleanRmOnlyUninstalled,pamac_config_set_clean_rm_only_uninstalled(m_handle,cleanRmOnlyUninstalled))
    PAMAC_QT_INT_PROPERTY_GET_SET(maxParallelDownloads,pamac_config_get_max_parallel_downloads(m_handle),
                                   setMaxParallelDownloads,pamac_config_set_max_parallel_downloads(m_handle,uint(maxParallelDownloads)))

    PAMAC_QT_BOOL_PROPERTY_GET_SET(checkAurVCSUpdates,pamac_config_get_check_aur_vcs_updates(m_handle),
                                   setCheckAurVCSUpdates,pamac_config_set_check_aur_vcs_updates(m_handle,checkAurVCSUpdates))

    PAMAC_QT_BOOL_PROPERTY_GET_SET(enableDowngrade,pamac_config_get_enable_downgrade(m_handle),
                                   setEnableDowngrade,pamac_config_set_enable_downgrade(m_handle,enableDowngrade))

    PAMAC_QT_INT_PROPERTY_GET_SET(refreshPeriod,pamac_config_get_refresh_period(m_handle),
                                   setRefreshPeriod,pamac_config_set_refresh_period(m_handle,uint(refreshPeriod)))

    PAMAC_QT_INT_PROPERTY_GET_SET(cleanKeepNumPkgs,pamac_config_get_clean_keep_num_pkgs(m_handle),
                              setCleanKeepNumPkgs,pamac_config_set_clean_keep_num_pkgs(m_handle,size_t(cleanKeepNumPkgs)))

    PAMAC_QT_STRING_PROPERTY_GET_SET(aurBuildDirectory,pamac_config_get_aur_build_dir(m_handle),
                                     setAurBuildDirectory,pamac_config_set_aur_build_dir(m_handle,aurBuildDirectory.toUtf8()))
    operator PamacConfig*(){
        return m_handle;
    }
    private:
        PamacConfig* m_handle;
};
} // namespace LibQPamac
