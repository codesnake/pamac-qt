#include <utility>
#include "NotificationServiceModule.h"
#include <Notification.h>
#include <Notifier.h>
#include <DesktopProgress.h>
#include <QtQml/QQmlEngine>
#include <libnotify/notify.h>
NotificationServiceModule::NotificationServiceModule(QString appName): m_appName(std::move(appName))
{
    notify_init(m_appName.toUtf8());
}

void NotificationServiceModule::registerTypes(const char *uri)
{
    qmlRegisterType<DesktopProgress>("NotificationServices",1,0,"DesktopProgress");
    qmlRegisterType<Notification>("NotificationServices",1,0,"Notification");
    qmlRegisterSingletonType<Notifier>("NotificationServices",1,0,"Notifier",
                                       [](QQmlEngine * engine,QJSEngine *scriptEngine)->QObject*{
       return new Notifier("system-software-install",engine);
    });
}
