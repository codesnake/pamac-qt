#include "AsyncHelpers.h"

QmlFuture::QmlFuture(QmlFutureImpl *implementation):
    implementation(std::shared_ptr<QmlFutureImpl>(implementation,[](QmlFutureImpl* impl){
                       if (impl->m_isRunning) {
                        impl->m_isRunning=false;
                       } else {
                        delete impl;
                       }
                   })){}

QVariant QmlFuture::result() const
{
    return implementation->futureData;
}

void QmlFuture::setResult(QVariant future){
    implementation->setFuture(std::move(future));
}

void QmlFutureImpl::setFuture(QVariant future){
    futureData = std::move(future);
    m_isRunning = false;
    if(watcher!=nullptr){
        emit watcher->finished(futureData);
    }
}

bool QmlFutureImpl::isRunning() const
{
    return m_isRunning;
}

bool QmlFuture::isRunning() const
{
    return implementation->m_isRunning;
}

void QmlFuture::setRunning(bool isRunning)
{
    implementation->m_isRunning = isRunning;
}

