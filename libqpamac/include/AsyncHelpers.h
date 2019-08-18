#pragma once
#include <QQuickItem>
#include <QGuiApplication>
#include <QCursor>
#include <memory>
#include <mutex>
class QmlFutureWatcher;
class Database;
struct QmlFutureImpl{
    std::mutex mutex;
    friend struct GenericQmlFuture;
    void setFuture(QVariant future);

public:
    bool isRunning() const;

private:
    bool m_isRunning = true;

    QVariant futureData;
    QmlFutureWatcher* watcher = nullptr;
};
struct GenericQmlFuture{
    friend class QmlFutureWatcher;
    Q_GADGET

public:
    GenericQmlFuture(QmlFutureImpl* implementation);
    GenericQmlFuture()=default;
    QVariant result() const;
    void setResult(QVariant future);
    bool isRunning() const;
    void setRunning(bool isRunning);

private:
    QmlFutureWatcher* getWatcher(){
        return implementation->watcher;
    }
    void setWatcher(QmlFutureWatcher* watcher){
        implementation->watcher=watcher;
    }
    std::shared_ptr<QmlFutureImpl> implementation;
};
Q_DECLARE_METATYPE(GenericQmlFuture)

class QmlFutureWatcher:public QQuickItem{
    Q_OBJECT
    Q_PROPERTY(GenericQmlFuture future READ future WRITE setFuture NOTIFY futureChanged)
    Q_PROPERTY(bool running MEMBER m_running)
public:
    explicit QmlFutureWatcher(QQuickItem* parent = nullptr):
        QQuickItem (parent)
    {
        connect(this,&QmlFutureWatcher::finished,this,&QmlFutureWatcher::reset);QGuiApplication::restoreOverrideCursor();
    }
    GenericQmlFuture future() const
    {
        return m_future;
    }

public Q_SLOTS:
    void setFuture(GenericQmlFuture future);
    inline Q_INVOKABLE void reset(){
        this->setProperty("running",false);



        while(QGuiApplication::overrideCursor()!=nullptr){
            QGuiApplication::restoreOverrideCursor();
        }
    }
Q_SIGNALS:
    void finished(QVariant result);
    void futureChanged(GenericQmlFuture future);
private:
    GenericQmlFuture m_future;
    bool m_running;
};
