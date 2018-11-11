#pragma once
#include "pamac.h"
#include <QObject>
#include <QtDebug>
#include <memory>
#define PAMAC_QT_STRING_PROPERTY_GET(name,method)\
    Q_PROPERTY(QString name READ name)\
    QString name() const\
{\
    return QString::fromUtf8(method);\
    }
#define PAMAC_QT_STRING_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(QString name READ name WRITE setName)\
    QString name() const\
{\
    return QString::fromUtf8(method);\
    }\
    void setName(const QString& str)\
{\
    setMethod(str.toUtf8());\
    }
#define PAMAC_QT_ULONG_PROPERTY_GET(name,method)\
    Q_PROPERTY(ulong name READ name)\
    ulong name() const\
{\
    return method;\
    }
#define PAMAC_QT_BOOL_PROPERTY_GET_SET(name,method,setName,setMethod)\
    Q_PROPERTY(bool name READ name WRITE setName)\
    bool name() const\
{\
    bool val = method;\
    qDebug()<<val;\
    return val;\
    }\
    void setName(bool name)\
{\
    setMethod;\
    }
namespace PamacQt {

class Config
{
    Q_GADGET
public:
    Config(PamacConfig* cfg):m_config(std::shared_ptr<PamacConfig>(cfg,g_object_unref))
    {

    }
    Config(const QString& str):
        m_config(std::shared_ptr<PamacConfig>(pamac_config_new(str.toUtf8()),g_object_unref))
    {

    }
    Config(){}

    PAMAC_QT_STRING_PROPERTY_GET(confPath,pamac_config_get_conf_path(m_config.get()))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(recurse,pamac_config_get_recurse(m_config.get()),
                                   setRecurse,pamac_config_set_recurse(m_config.get(),recurse))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(noUpdateHideIcon,pamac_config_get_no_update_hide_icon(m_config.get()),
                                   setNoUpdateHideIcon,pamac_config_set_no_update_hide_icon(m_config.get(),noUpdateHideIcon))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(enableAur,pamac_config_get_enable_aur(m_config.get()),
                                   setEnableAur,pamac_config_set_enable_aur(m_config.get(),enableAur))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(checkAurUpdates,pamac_config_get_check_aur_updates(m_config.get()),
                                   setCheckAurUpdates,pamac_config_set_check_aur_updates(m_config.get(),checkAurUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(downloadUpdates,pamac_config_get_download_updates(m_config.get()),
                                   setDownloadUpdates,pamac_config_set_download_updates(m_config.get(),downloadUpdates))
    PAMAC_QT_BOOL_PROPERTY_GET_SET(cleanRmOnlyUninstalled,pamac_config_get_clean_rm_only_uninstalled(m_config.get()),
                                   setCleanRmOnlyUninstalled,pamac_config_set_clean_rm_only_uninstalled(m_config.get(),cleanRmOnlyUninstalled))

    operator PamacConfig*(){
        return m_config.get();
    }
    private:
        std::shared_ptr<PamacConfig> m_config;
};
} // namespace PamacQt
