#include "DesktopProgress.h"
#include <QtDBus>
void send_unity_launcher_entry_message(const QVariantMap& message){
    QDBusMessage signal = QDBusMessage::createSignal("/",
                                                     "com.canonical.Unity.LauncherEntry",
                                                     "Update");

    signal << "application://pamac-qt.desktop";

    signal << message;

    QDBusConnection::sessionBus().send(signal);
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
    send_unity_launcher_entry_message({{"progress-visible",show}});
}

void DesktopProgress::setProgress(double progress)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_progress, progress)) {
        return;
}

    m_progress = progress;
    Q_EMIT progressChanged(m_progress);

    send_unity_launcher_entry_message({{"progress",progress}});
}

void DesktopProgress::setIndeterminate(bool indeterminate)
{
    if (m_indeterminate == indeterminate) {
        return;
}

    m_indeterminate = indeterminate;
    Q_EMIT indeterminateChanged(m_indeterminate);

    send_unity_launcher_entry_message({{}});
}
DesktopProgress::DesktopProgress(QQuickItem* parent):QQuickItem(parent){
}
