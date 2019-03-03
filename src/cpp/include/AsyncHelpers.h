#pragma once
#include <QQuickItem>
#include <QApplication>
#include <memory>
class QmlFutureWatcher;
class Database;
struct QmlFutureImpl{
    friend struct QmlFuture;
    void setFuture(QVariant future);

public:
    bool isRunning() const;

private:
    bool m_isRunning = true;

    QVariant futureData;
    QmlFutureWatcher* watcher = nullptr;
};
struct QmlFuture{
    friend class QmlFutureWatcher;
    Q_GADGET
public:
    QmlFuture(QmlFutureImpl* implementation);
    QmlFuture()=default;
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
Q_DECLARE_METATYPE(QmlFuture)
class QmlFutureWatcher:public QQuickItem{
    Q_OBJECT
    Q_PROPERTY(QmlFuture future READ future WRITE setFuture NOTIFY futureChanged)
public:
    explicit QmlFutureWatcher(QQuickItem* parent = nullptr):
        QQuickItem (parent)
    {
        connect(this,&QmlFutureWatcher::finished,this,&QmlFutureWatcher::reset);QApplication::restoreOverrideCursor();
    }
    QmlFuture future() const
    {
        return m_future;
    }

public slots:
    void setFuture(QmlFuture future)
    {

        if (m_future.implementation == future.implementation) {
            return;
        }
        if(m_future.implementation!=nullptr){
            m_future.setWatcher(nullptr);
        }
        QApplication::setOverrideCursor(Qt::WaitCursor);

        future.setWatcher(this);

        emit futureChanged(future);

        if(!future.isRunning()) {
            emit finished(future.result());
        }
    }
    inline Q_INVOKABLE void reset(){
        m_future = QmlFuture();

        while(QApplication::overrideCursor()!=nullptr){
            QApplication::restoreOverrideCursor();
        }
    }
signals:
    void finished(QVariant result);
    void futureChanged(QmlFuture future);
private:
    QmlFuture m_future;
};
