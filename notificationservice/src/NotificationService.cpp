#include "NotificationService.h"
#include "UnityProgressNotifier.h"
#include <QtDBus>
#include <vector>
NotificationService::NotificationService(QObject *parent) : QObject(parent)
{

}

NotificationService* NotificationService::createDefault(QObject *parent)
{

    //Probe for notification capabilites using DBus
    auto service = new NotificationService(parent);

    QDBusInterface unityLauncherEntryInterface("com.canonical.Unity","/");

    if(unityLauncherEntryInterface.isValid()){
        service->m_notifiers.push_back(std::unique_ptr<AbstractNotifier>(new UnityProgressNotifier));
    }

    return service;
}

void NotificationService::setProgress(double progress)
{
    for (auto& notifier : m_notifiers) {
        notifier->setProgress(progress);
    }
}

void NotificationService::setMessage(const QString &message)
{
    for (auto& notifier : m_notifiers) {
        notifier->setMessage(message);
    }
}

void NotificationService::setVisible(bool visible)
{
    for (auto& notifier : m_notifiers) {
        notifier->setVisible(visible);
    }
}
