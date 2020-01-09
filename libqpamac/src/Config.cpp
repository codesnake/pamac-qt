#include <Config.h>

QStringList LibQPamac::Config::getIgnorePkgs()
{
    return Utils::gHashToQMap<QString,QString>(pamac_config_get_ignorepkgs(m_handle),[](void* key,void* value){

        auto keyString = QString::fromUtf8(reinterpret_cast<gchar*>(key));
        auto valueString = QString::fromUtf8(reinterpret_cast<gchar*>(value));

        return QPair(keyString,valueString);
    }).values();
}
