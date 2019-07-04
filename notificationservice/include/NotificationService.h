#pragma once

#include <QObject>
#include <AbstractNotifier.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <memory>
class NotificationService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progress WRITE setProgress)
public:
    class NotificationServiceBuilder{
    public:
        NotificationServiceBuilder();
        NotificationServiceBuilder* probeDBus();
        NotificationServiceBuilder* addTrayIcon(QSystemTrayIcon *icon);
        NotificationService* build(QObject* parent = nullptr);
    private:
        NotificationService* m_managedObject;
    };
    explicit NotificationService(QObject *parent = nullptr);
signals:

public slots:
    Q_INVOKABLE void setProgress(double progress);
    Q_INVOKABLE void setMessage(const QString& message);
    Q_INVOKABLE void setVisible(bool visible);
private:
    std::vector<std::unique_ptr<AbstractNotifier>> m_notifiers;
};
