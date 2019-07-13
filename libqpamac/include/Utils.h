#pragma once
#include "pamac.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <QVariantList>
#include <QStringList>
#include <QDateTime>

namespace LibQPamac::Utils{

#define PAMAC_QT_STRING_PROPERTY_GET(name,method)\
    Q_PROPERTY(QString name READ name CONSTANT)\
    inline QString name() \
{\
    return QString::fromUtf8(method);\
}
#define PAMAC_QT_DATETIME_PROPERTY_GET(name,method)\
    Q_PROPERTY(QDateTime name READ name CONSTANT)\
    inline QDateTime name()\
{\
    return QDateTime::fromTime_t(method);\
}
#define PAMAC_QT_STRING_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(QString getName READ getName WRITE setName)\
    inline QString getName()\
{\
    return QString::fromUtf8(getMethod);\
}\
    inline void setName(const QString& getName)\
{\
    setMethod;\
}

#define PAMAC_QT_URL_PROPERTY_GET(name,method)\
    Q_PROPERTY(QUrl name READ name CONSTANT)\
    inline QUrl name()\
{\
    return QUrl(method);\
}

#define PAMAC_QT_INT_PROPERTY_GET(name,method)\
    Q_PROPERTY(int name READ name CONSTANT)\
    inline int name()\
{\
    return int(method);\
}
#define PAMAC_QT_INT_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(int getName READ getName WRITE setName)\
    inline int getName()\
{\
    return int(getMethod);\
}\
    inline void setName(int getName){\
    setMethod;\
}
#define PAMAC_QT_UINT_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(int getName READ getName WRITE setName)\
    inline uint getName() \
{\
    return uint(getMethod);\
}\
    inline void setName(uint getName){\
    setMethod;\
}
#define PAMAC_QT_DOUBLE_PROPERTY_GET(name,method)\
    Q_PROPERTY(double name READ name CONSTANT)\
    inline double name()\
{\
    return double(method);\
}
#define PAMAC_QT_STRINGLIST_PROPERTY_GET(name,method)\
    Q_PROPERTY(QStringList name READ name CONSTANT)\
    inline QStringList name()\
{\
    QStringList result;\
    auto tmp = method;\
    for(auto el = g_list_first(tmp);el!=nullptr;el=el->next){\
    result.append((QString::fromUtf8(static_cast<char*>(el->data))));\
}\
    return result;\
}


#define PAMAC_QT_BOOL_PROPERTY_GET_SET(name,method,setName,setMethod)\
    Q_PROPERTY(bool name READ name WRITE setName)\
    inline bool name()\
{\
    return method;\
}\
    inline void setName(bool name)\
{\
    setMethod;\
}



std::vector<char*> qStringListToCStringVector(const QStringList&);
QStringList cStringArrayToQStringList(char** arr,int size);
GList* qVariantListToGList(const QVariantList& lst);
GVariant* qVariantToGVariant(const QVariant&);
QStringList gListToQStringList(GList* lst,bool freeOriginal = false);

template <typename T>
QList<T> gListToQList(GList* list,std::function<T(void*)> wrapFunction,GDestroyNotify freeFunc = nullptr)
{
    QList<T> result;
    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(wrapFunction(el->data));
    }

    if(freeFunc){
        g_list_free_full(list,freeFunc);
    }

    return result;
}

// https://stackoverflow.com/a/48368508

template <class F>
struct lambda_traits : lambda_traits<decltype(&F::operator())>
{ };

template <typename F, typename R, typename... Args>
struct lambda_traits<R(F::*)(Args...)> : lambda_traits<R(F::*)(Args...) const>
{ };

template <class F, class R, class... Args>
struct lambda_traits<R(F::*)(Args...) const> {
    using pointer = typename std::add_pointer<R(Args...)>::type;

    static pointer cify(F&& f) {
        static F fn = std::forward<F>(f);
        return [](Args... args) {
            return fn(std::forward<Args>(args)...);
        };
    }
};

template <class F>
inline typename lambda_traits<F>::pointer cify(F&& f) {
    return lambda_traits<F>::cify(std::forward<F>(f));
}

} // namespace LibQPamac::Utils
