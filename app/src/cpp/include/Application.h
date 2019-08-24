#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
class Application: public QObject
{
    Q_OBJECT
public:
    Application(QObject* parent = nullptr);
    static QMap<QString,QObject*> m_objects;

    int exec();

public slots:
    void loadMainWindow();
private:
    QQmlApplicationEngine m_engine;
    QCommandLineParser m_parser;

private slots:
    void saveGeometry();
    void restoreGeometry();
    void loadOpenWithDialog();
    void loadTrayIcon();
    void registerCommandLineOptions();
    void registerModules();
    bool registerDbusService();
};
