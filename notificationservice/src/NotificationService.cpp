#include "NotificationService.h"
#include "UnityProgressNotifier.h"
#include "TrayIconNotifier.h"
#include <QApplication>
#include <QtDBus>
#include <vector>
NotificationService::NotificationService(QObject *parent) : QObject(parent)
{

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

NotificationService::NotificationServiceBuilder::NotificationServiceBuilder():m_managedObject(new NotificationService)
{

}

NotificationService::NotificationServiceBuilder *NotificationService::NotificationServiceBuilder::probeDBus()
{
    //Probe for notification capabilites using DBus
    auto service = m_managedObject;

    QDBusInterface unityLauncherEntryInterface("com.canonical.Unity","/");

    if(unityLauncherEntryInterface.isValid()){
        service->m_notifiers.push_back(std::unique_ptr<AbstractNotifier>(new UnityProgressNotifier));
    }

    return this;
}

NotificationService::NotificationServiceBuilder *NotificationService::NotificationServiceBuilder::addTrayIcon(QSystemTrayIcon* icon)
{
    auto trayNotifier = new TrayIconNotifier(icon);

    trayNotifier->trayItem()->show();
    m_managedObject->m_notifiers.push_back(std::unique_ptr<AbstractNotifier>(trayNotifier));
    return this;
}

NotificationService *NotificationService::NotificationServiceBuilder::build(QObject *parent)
{
    m_managedObject->setParent(parent);
    return m_managedObject;
}
