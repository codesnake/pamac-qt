#include "AsyncHelpers.h"

GenericQmlFuture::GenericQmlFuture(QmlFutureImpl *implementation):
    implementation(std::shared_ptr<QmlFutureImpl>(implementation,[](QmlFutureImpl* impl){
                       if (impl->m_isRunning) {
                        impl->m_isRunning=false;
                       } else {
                        delete impl;
                       }
                   })){}

QVariant GenericQmlFuture::result() const
{
    return implementation->futureData;
}

void GenericQmlFuture::setResult(QVariant future){
    implementation->setFuture(std::move(future));
}

void QmlFutureImpl::setFuture(QVariant future){
    futureData = std::move(future);
    m_isRunning = false;
    if(watcher!=nullptr){
        Q_EMIT watcher->finished(futureData);
    }
}

bool QmlFutureImpl::isRunning() const
{
    return m_isRunning;
}

bool GenericQmlFuture::isRunning() const
{
    return implementation->m_isRunning;
}

void GenericQmlFuture::setRunning(bool isRunning)
{
    implementation->m_isRunning = isRunning;
}

