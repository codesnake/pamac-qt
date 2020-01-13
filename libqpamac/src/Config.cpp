#include <Config.h>

QStringList LibQPamac::Config::getIgnorePkgs()
{
    return Utils::gHashToQMap<QString,QString>(pamac_config_get_ignorepkgs(m_handle),[](void* key,void* value){

        auto keyString = QString::fromUtf8(reinterpret_cast<gchar*>(key));
        auto valueString = QString::fromUtf8(reinterpret_cast<gchar*>(value));

        return QPair(keyString,valueString);
    }).values();
}

void LibQPamac::Config::save()
{
    pamac_config_save(m_handle);
}

void LibQPamac::Config::reload()
{
    pamac_config_reload(m_handle);
}
