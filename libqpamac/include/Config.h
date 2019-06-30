#pragma once
#include "pamac.h"
#include <QObject>
#include <QtDebug>
#include <memory>
#include "Utils.h"

namespace LibQPamac{

class Config:public PamacConfig
{
    Q_GADGET
public:
    Config(PamacConfig* cfg)
    {
        *static_cast<PamacConfig*>(this) = *cfg;
    }
    Config(const QString& str)
    {
        *static_cast<PamacConfig*>(this) = *pamac_config_new(str.toUtf8());
    }
    Config()=default;

    PAMAC_QT_STRING_PROPERTY_GET(confPath,pamac_config_get_conf_path(this))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(recurse,pamac_config_get_recurse(this),
                                   setRecurse,pamac_config_set_recurse(this,recurse))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(noUpdateHideIcon,pamac_config_get_no_update_hide_icon(this),
                                   setNoUpdateHideIcon,pamac_config_set_no_update_hide_icon(this,noUpdateHideIcon))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(enableAur,pamac_config_get_enable_aur(this),
                                   setEnableAur,pamac_config_set_enable_aur(this,enableAur))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(checkAurUpdates,pamac_config_get_check_aur_updates(this),
                                   setCheckAurUpdates,pamac_config_set_check_aur_updates(this,checkAurUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(downloadUpdates,pamac_config_get_download_updates(this),
                                   setDownloadUpdates,pamac_config_set_download_updates(this,downloadUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(cleanRmOnlyUninstalled,pamac_config_get_clean_rm_only_uninstalled(this),
                                   setCleanRmOnlyUninstalled,pamac_config_set_clean_rm_only_uninstalled(this,cleanRmOnlyUninstalled))
    PAMAC_QT_INT_PROPERTY_GET_SET(maxParallelDownloads,pamac_config_get_max_parallel_downloads(this),
                                   setMaxParallelDownloads,pamac_config_set_max_parallel_downloads(this,uint(maxParallelDownloads)))

    PAMAC_QT_BOOL_PROPERTY_GET_SET(checkAurVCSUpdates,pamac_config_get_check_aur_vcs_updates(this),
                                   setCheckAurVCSUpdates,pamac_config_set_check_aur_vcs_updates(this,checkAurVCSUpdates))

    PAMAC_QT_BOOL_PROPERTY_GET_SET(enableDowngrade,pamac_config_get_enable_downgrade(this),
                                   setEnableDowngrade,pamac_config_set_enable_downgrade(this,enableDowngrade))

    PAMAC_QT_INT_PROPERTY_GET_SET(refreshPeriod,pamac_config_get_refresh_period(this),
                                   setRefreshPeriod,pamac_config_set_refresh_period(this,uint(refreshPeriod)))

    PAMAC_QT_INT_PROPERTY_GET_SET(cleanKeepNumPkgs,pamac_config_get_clean_keep_num_pkgs(this),
                              setCleanKeepNumPkgs,pamac_config_set_clean_keep_num_pkgs(this,size_t(cleanKeepNumPkgs)))

    PAMAC_QT_STRING_PROPERTY_GET_SET(aurBuildDirectory,pamac_config_get_aur_build_dir(this),
                                     setAurBuildDirectory,pamac_config_set_aur_build_dir(this,aurBuildDirectory.toUtf8()))

};
} // namespace LibQPamac
