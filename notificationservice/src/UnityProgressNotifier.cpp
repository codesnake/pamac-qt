#include "UnityProgressNotifier.h"
#include "QtDBus"

void send_unity_launcherentry_message(const QVariantMap& message){
    QDBusMessage signal = QDBusMessage::createSignal("/",
                                                     "com.canonical.Unity.LauncherEntry",
                                                     "Update");

    signal << "application://pamac-qt.desktop";

    signal << message;

    QDBusConnection::sessionBus().send(signal);
}

UnityProgressNotifier::UnityProgressNotifier(QObject* parent):QObject(parent)
{

}

void UnityProgressNotifier::setVisible(bool visible)
{
    send_unity_launcherentry_message({{"progress-visible",visible}});
}


void UnityProgressNotifier::setProgress(double progress)
{
    send_unity_launcherentry_message({{"progress",progress}});
}

void UnityProgressNotifier::setMessage(const QString &message)
{
    Q_UNUSED(message);
}
