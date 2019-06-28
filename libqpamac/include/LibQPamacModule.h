#pragma once
#include <QQmlExtensionPlugin>
class LibQPamacModule:public QQmlExtensionPlugin
{
public:
    LibQPamacModule()=default;

    // QQmlTypesExtensionInterface interface
public:
    void registerTypes(const char *uri = nullptr) override;
};

