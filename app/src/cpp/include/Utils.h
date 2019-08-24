#pragma once
#include <QObject>
#include <functional>
//https://stackoverflow.com/a/45244152
namespace Utils {

class LambdaHelper : public QObject {
    Q_OBJECT
    std::function<void()> m_fun;
public:
    LambdaHelper(std::function<void()> && fun, QObject * parent = {}) :
        QObject(parent),
        m_fun(std::move(fun)) {}
    Q_SLOT void call() { m_fun(); }
    static QMetaObject::Connection connect(
            QObject * sender, const char * signal, std::function<void()> && fun)
    {
        if (sender == nullptr) {
            return QMetaObject::Connection();
        }
        return QObject::connect(sender, signal,
                                new LambdaHelper(std::move(fun), sender), SLOT(call()));
    }
};
}//namespace Utils
