#include <utility>

#include "Notifier.h"
#include <libnotify/notify.h>
#include <Notification.h>
Notifier::Notifier(QString  appIcon, QObject *parent) : QObject(parent), m_iconName(std::move(appIcon))
{
}

void Notifier::send(const QVariantMap& options)
{
    auto notification = new Notification();
    notification->setDestroyOnClose(true);
    notification->setBody(options["body"].toString());
    notification->setSummary(options["summary"].toString());

    auto iconName = options["iconName"];
    if(iconName.isValid() && iconName.canConvert<QString>()){
        notification->setIconName(iconName.toString());
    } else if(!m_iconName.isEmpty()){
        notification->setIconName(m_iconName);
    }

    notification->show();

}
