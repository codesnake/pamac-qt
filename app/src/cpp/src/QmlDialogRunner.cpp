#include "QmlDialogRunner.h"
#include <QQmlComponent>
#include <QQuickView>
#include <QtDebug>
#include <QEventLoop>
#include <QQuickItem>
#include <QQmlApplicationEngine>
QmlDialogRunner::QmlDialogRunner(QObject* parent) : QObject(parent)
{

}

QVariantMap QmlDialogRunner::exec(const QUrl &qmlFile,const QVariantMap& propertiesMap)
{
    CloseEventFilter filter;
    auto engine = qobject_cast<QQmlApplicationEngine*>(parent());
    auto mainWindow = engine->rootObjects()[0];


    QQuickView view(engine,nullptr);
    view.installEventFilter(&filter);
    view.setSource(qmlFile);
    view.show();
    QObject *object = view.rootObject();

    for (auto it = propertiesMap.keyValueBegin();it!=propertiesMap.keyValueEnd();++it) {
        if((*it).second.isValid()){
            object->setProperty((*it).first.toUtf8(),(*it).second);
        }
    }
    view.setTitle(object->property("title").toString());
    connect(object,SIGNAL(close()),&view,SLOT(close()));

    bool ok = false;
    auto minHeight = object->property("minimumHeight").toInt(&ok);
    view.setMinimumHeight(ok?minHeight:400);
    auto minWidth = object->property("minimumWidth").toInt(&ok);
    view.setMinimumWidth(ok?minWidth:600);

    int x = mainWindow->property("x").toInt()+(mainWindow->property("width").toInt()/2)-view.minimumWidth()/2;
    int y = mainWindow->property("y").toInt()+(mainWindow->property("height").toInt()/2)-view.minimumHeight()/2;

    view.setPosition(x,y);

    QEventLoop loop;
    connect(&filter,&CloseEventFilter::closing,&loop,&QEventLoop::quit);
    loop.exec();
    QVariantMap result = propertiesMap;
    for (auto it = result.keyValueBegin();it!=result.keyValueEnd();++it) {
        (*it).second = object->property((*it).first.toUtf8());
    }
    return result;
}
