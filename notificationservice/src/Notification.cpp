#include "Notification.h"
#include <glib.h>
#include <QtDebug>

Notification::Notification(QQuickItem *parent): QQuickItem(parent)
{
    for(auto signal : {
        &Notification::summaryChanged,
        &Notification::bodyChanged,
        &Notification::iconNameChanged}){

        connect(this,signal,this,&Notification::updateNotification);

    }
}

void Notification::setSummary(const QString &summary)
{
    if (m_summary == summary) {
        return;
    }

    m_summary = summary;
    Q_EMIT summaryChanged(m_summary);
}

void Notification::setBody(const QString &body)
{
    if (m_body == body) {
        return;
    }

    m_body = body;
    Q_EMIT bodyChanged(m_body);
}

void Notification::setIconName(const QString &iconName)
{
    if (m_iconName == iconName) {
        return;
    }

    m_iconName = iconName;
    Q_EMIT iconNameChanged(m_iconName);
}

bool Notification::show()
{
    return bool(notify_notification_show(m_handle,nullptr));
}

bool Notification::hide()
{
    return bool(notify_notification_close(m_handle,nullptr));
}

void Notification::setTimeout(int timeout)
{
    if (m_timeout == timeout) {
        return;
    }

    m_timeout = timeout;
    notify_notification_set_timeout(m_handle,timeout);
    Q_EMIT timeoutChanged(m_timeout);
}

void Notification::setDestroyOnClose(bool destroy)
{
    if(destroy){
        if(parent()!=nullptr){
            qWarning() << "Cannot destroy an attached notification on close!";
        }
        m_destroyConnectionId = g_signal_connect(m_handle,"closed",
                                                 reinterpret_cast<GCallback>(+[](GObject* obj,bool success,Notification* t){
                                                     t->deleteLater();
                                                 }),this);
    } else{
        g_signal_handler_disconnect(m_handle,m_destroyConnectionId);
    }

}

void Notification::setActions(const QStringList &actions)
{
    if (m_actions == actions)
        return;
    notify_notification_clear_actions(m_handle);
    for(auto& string : actions){
        notify_notification_add_action(m_handle,string.toLower().toUtf8(),string.toUtf8(),[](NotifyNotification *notification,char* id,gpointer data){
            auto that = reinterpret_cast<Notification*>(data);
            Q_EMIT that->actionClicked(id);
        },this,nullptr);
    }


    m_actions = actions;
    Q_EMIT actionsChanged(m_actions);
}

void Notification::updateNotification()
{
    if(m_handle==nullptr){
        m_handle = notify_notification_new(m_summary.toUtf8(),m_body.toUtf8(),m_iconName.toUtf8());
    } else{
        notify_notification_update(m_handle,m_summary.toUtf8(),m_body.toUtf8(),m_iconName.toUtf8());
    }
}
