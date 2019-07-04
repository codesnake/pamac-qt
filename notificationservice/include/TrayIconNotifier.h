#pragma once

#include <QWidget>
#include <QSystemTrayIcon>
#include <AbstractNotifier.h>
class TrayIconNotifier:public AbstractNotifier
{
public:
    TrayIconNotifier(QSystemTrayIcon* icon);

    // AbstractNotifier interface
    QSystemTrayIcon* trayItem() const;
    void setTrayItem(QSystemTrayIcon* trayItem);

public slots:
    void setVisible(bool visible) override;
    void setProgress(double progress) override;
    void setMessage(const QString &message) override;
private:
    QSystemTrayIcon* m_trayItem;
};
