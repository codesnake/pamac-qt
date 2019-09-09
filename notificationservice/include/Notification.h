#pragma once

#include <libnotify/notification.h>
#include <QtQuick/QQuickItem>
#include <QtQml/QJSValue>
#include <QIcon>

class Notification: public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
    Q_PROPERTY(QStringList actions READ actions WRITE setActions NOTIFY actionsChanged)


    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)


public:
    Notification(QQuickItem* parent = nullptr);
    ~Notification() override {
        g_object_unref(m_handle);
    }

    QString summary() const
    {
        return m_summary;
    }

    QString body() const
    {
        return m_body;
    }

    QString iconName() const
    {
        return m_iconName;
    }

    int timeout() const
    {
        return m_timeout;
    }

    QStringList actions() const
    {
        return m_actions;
    }

public Q_SLOTS:
    void setSummary(const QString& summary);

    void setBody(const QString& body);

    void setIconName(const QString& iconName);

    bool show();
    bool hide();

    void setTimeout(int timeout);

    void setDestroyOnClose(bool destroy);

    void setActions(const QStringList& actions);

Q_SIGNALS:
    void summaryChanged(QString summary);

    void bodyChanged(QString body);

    void iconNameChanged(QString iconName);

    void timeoutChanged(int timeout);

    void actionClicked(QByteArray actionId);

    void actionsChanged(QStringList actions);

private:
    NotifyNotification* m_handle = nullptr;
    QString m_summary;
    QString m_body;
    QString m_iconName;

    ulong m_destroyConnectionId;
    int m_timeout;

    void updateNotification();
    QStringList m_actions;
};
