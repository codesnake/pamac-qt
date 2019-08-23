#pragma once

#include <QtQml/QQmlExtensionPlugin>

class NotificationServiceModule:public QQmlExtensionPlugin
{
public:
    NotificationServiceModule(QString  appName);

    // QQmlTypesExtensionInterface interface
public:
    void registerTypes(const char *uri) override;

private:
    QString m_appName;
};
