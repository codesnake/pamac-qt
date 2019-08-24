#pragma once
#include <QObject>
#include <QUrl>
#include <QEventLoop>
#include <QEvent>
#include <QMap>
#include <QVariant>
#include <QQuickItem>
class CloseEventFilter:public QObject{
    Q_OBJECT
public:
    bool eventFilter(QObject *obj, QEvent *event) override{
        if(event->type()==QEvent::Close){
            emit closing();
            return true;
        }
        return QObject::eventFilter(obj, event);
    }
signals:
    void closing();
};
class QmlDialogRunner:public QObject
{
    Q_OBJECT
public:
    QmlDialogRunner(QString  parentName, QObject* parent = nullptr);
    Q_INVOKABLE QVariantMap exec(const QUrl& qmlFile,const QVariantMap& properties = QVariantMap());
private:
    QString m_parentName;
};
