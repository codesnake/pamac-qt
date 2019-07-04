#include "TrayIconNotifier.h"


void TrayIconNotifier::setVisible(bool visible)
{
}

void TrayIconNotifier::setProgress(double progress)
{
}

void TrayIconNotifier::setMessage(const QString &message)
{
}

TrayIconNotifier::TrayIconNotifier(QSystemTrayIcon *icon):m_trayItem(icon)
{

}

QSystemTrayIcon* TrayIconNotifier::trayItem() const
{
    return m_trayItem;
}

void TrayIconNotifier::setTrayItem(QSystemTrayIcon *trayItem)
{
    m_trayItem = trayItem;
}
