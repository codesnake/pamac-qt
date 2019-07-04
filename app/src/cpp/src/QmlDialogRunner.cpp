#include "QmlDialogRunner.h"
#include <QQmlComponent>
#include <QQuickView>
#include <QtDebug>
#include <QEventLoop>
#include <QQuickItem>
#include <QQmlEngine>
QmlDialogRunner::QmlDialogRunner(QObject* parent) : QObject(parent)
{

}

QVariantMap QmlDialogRunner::exec(const QUrl &qmlFile,QVariantMap propertiesMap)
{
    CloseEventFilter filter;
    QQuickView view(static_cast<QQmlEngine*>(parent()),nullptr);
    view.installEventFilter(&filter);
    view.setSource(qmlFile);
    view.show();
    QObject *object = view.rootObject();

    view.setTitle(object->property("title").toString());

    for (auto it = propertiesMap.keyValueBegin();it!=propertiesMap.keyValueEnd();++it) {
        object->setProperty((*it).first.toUtf8(),(*it).second);
    }
    connect(object,SIGNAL(close()),&view,SLOT(close()));
    QEventLoop loop;
    connect(&filter,&CloseEventFilter::closing,&loop,&QEventLoop::quit);
    loop.exec();
    for (auto it = propertiesMap.keyValueBegin();it!=propertiesMap.keyValueEnd();++it) {
        (*it).second = object->property((*it).first.toUtf8());
    }
    return propertiesMap;
}
