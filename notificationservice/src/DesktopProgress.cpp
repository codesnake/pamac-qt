#include "DesktopProgress.h"
#include <QApplication>
#include <QtDBus>
void send_unity_launcher_entry_message(const QVariantMap& message){
    QDBusMessage signal = QDBusMessage::createSignal("/",
                                                     "com.canonical.Unity.LauncherEntry",
                                                     "Update");

    signal << "application://pamac-qt.desktop";

    signal << message;

    QDBusConnection::sessionBus().send(signal);
}

QString kjob_get_job(){
    QDBusInterface interface("org.kde.JobViewServer", "/JobViewServer", "org.kde.JobViewServer", QDBusConnection::sessionBus());

    if(!interface.isValid()){
        return QString();
    }
    auto message = interface.call("requestView",QApplication::applicationName(),"system-software-install",0);
    qDebug()<<message.arguments();

    return message.arguments()[0].value<QDBusObjectPath>().path();
}
void kjob_update_progress(const QString& job, uint percent){
    if(job.isNull()){
        return;
    }
    QDBusInterface interface("org.kde.JobViewServer", job, "org.kde.JobViewV2", QDBusConnection::sessionBus());
    if(interface.isValid())
        interface.call("setPercent",percent);
}
void kjob_terminate(const QString& job){
    if(job.isNull()){
        return;
    }
    QDBusInterface interface("org.kde.JobViewServer", job, "org.kde.JobViewV2", QDBusConnection::sessionBus());

    if(interface.isValid()){
        interface.call("terminate","");
    }
}
void kjob_update_info(const QString& job, const QString& info){
    if(job.isNull()){
        return;
    }
    QDBusInterface interface("org.kde.JobViewServer", job, "org.kde.JobViewV2", QDBusConnection::sessionBus());

    if(interface.isValid()){
        interface.call("setInfoMessage",info);
    }
}


bool DesktopProgress::visible() const
{
    return m_show;
}

double DesktopProgress::progress() const
{
    return m_progress;
}

bool DesktopProgress::indeterminate() const
{
    return m_indeterminate;
}

void DesktopProgress::setShow(bool show)
{
    if (m_show == show) {
        return;
    }

    m_show = show;
    Q_EMIT showChanged(m_show);

    if(m_show){
        m_kjob = kjob_get_job();
        kjob_update_info(m_kjob,m_infoMessage);
        kjob_update_progress(m_kjob,uint(m_progress*100));
    } else{
        kjob_terminate(m_kjob);
    }

    send_unity_launcher_entry_message({{"progress-visible",show}});
}

void DesktopProgress::setProgress(double progress)
{
    if (qFuzzyCompare(m_progress, progress)) {
        return;
    }

    m_progress = progress;
    Q_EMIT progressChanged(m_progress);

    if(!m_kjob.isNull()){
        kjob_update_progress(m_kjob,uint(progress*100));
    }
    send_unity_launcher_entry_message({{"progress",progress}});
}

void DesktopProgress::setIndeterminate(bool indeterminate)
{
    if (m_indeterminate == indeterminate) {
        return;
    }

    m_indeterminate = indeterminate;


    Q_EMIT indeterminateChanged(m_indeterminate);
    if(!m_kjob.isNull()){
        if(m_indeterminate){
            kjob_update_progress(m_kjob,0);
        } else {
            kjob_update_progress(m_kjob,uint(m_progress*100));
        }
    }
}
DesktopProgress::DesktopProgress(QQuickItem* parent):QQuickItem(parent){
}

void DesktopProgress::setInfoMessage(const QString& infoMessage)
{
    if (m_infoMessage == infoMessage)
        return;

    m_infoMessage = infoMessage;
    kjob_update_info(m_kjob,infoMessage);
    Q_EMIT infoMessageChanged(m_infoMessage);
}
