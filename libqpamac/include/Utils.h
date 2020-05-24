#pragma once
#include "pamac.h"
#include <vector>
#include <functional>
#include <type_traits>
#include <QVariantList>
#include <QStringList>
#include <QDateTime>
#include <QtDebug>

namespace LibQPamac::Utils{

#define PAMAC_QT_STRING_PROPERTY_GET(name,method)\
    Q_PROPERTY(QString name READ name CONSTANT)\
    inline QString name() const \
{\
    return QString::fromUtf8(method);\
}
#define PAMAC_QT_DATETIME_PROPERTY_GET(name,method)\
    Q_PROPERTY(QDateTime name READ name CONSTANT)\
    inline QDateTime name()\
{\
    return QDateTime::fromTime_t(uint(method));\
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
    inline QUrl name() const\
{\
    return QUrl(method);\
}

#define PAMAC_QT_INT_PROPERTY_GET(name,method)\
    Q_PROPERTY(int name READ name CONSTANT)\
    inline int name() const\
{\
    return int(method);\
}
#define PAMAC_QT_INT_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(int getName READ getName WRITE setName)\
    inline int getName() const\
{\
    return int(getMethod);\
}\
    inline void setName(int getName){\
    setMethod;\
}
#define PAMAC_QT_UINT_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(int getName READ getName WRITE setName)\
    inline uint getName() const\
{\
    return uint(getMethod);\
}\
    inline void setName(uint getName){\
    setMethod;\
}
#define PAMAC_QT_DOUBLE_PROPERTY_GET(name,method)\
    Q_PROPERTY(double name READ name CONSTANT)\
    inline double name() const\
{\
    return double(method);\
}
#define PAMAC_QT_STRINGLIST_PROPERTY_GET(name,method)\
    Q_PROPERTY(QStringList name READ name CONSTANT)\
    inline QStringList name() const\
{\
    QStringList result;\
    auto tmp = method;\
    for(auto el = tmp;el!=nullptr;el=el->next){\
    result.append((QString::fromUtf8(static_cast<char*>(el->data))));\
}\
    return result;\
}


#define PAMAC_QT_BOOL_PROPERTY_GET_SET(name,method,setName,setMethod)\
    Q_PROPERTY(bool name READ name WRITE setName)\
    inline bool name() const\
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
QStringList gsListToQStringList(GSList* lst,bool freeOriginal = false);

template <typename T>
QList<T> gSListToQList(GSList* list,std::function<T(void*)> wrapFunction,GDestroyNotify freeFunc = nullptr)
{
    QList<T> result;
    for(auto el = list;el!=nullptr;el=el->next)
    {
        result.append(wrapFunction(el->data));
    }

    if(freeFunc){
        g_slist_free_full(list,freeFunc);
    }

    return result;
}
template <typename T1,typename T2>
QMap<T1,T2> gHashToQMap(GHashTable* table,std::function<QPair<T1,T2>(void*,void*)> wrapFunction,GHRFunc freeFunc = nullptr)
{
    QMap<T1,T2> result;
    GList* list = g_hash_table_get_keys(table);

    for(auto el = list;el!=nullptr;el=el->next)
    {
        QPair<T1,T2> pair = wrapFunction(el->data,g_hash_table_lookup(table,el->data));
        result[pair.first] = pair.second;
    }

    if(freeFunc){
        g_hash_table_foreach_remove(table,freeFunc,nullptr);
    }

    return result;
}



#define PAMAC_QT_PACKAGE_TO_VARIANT_WRAP(PackageType)\
    [](void* data){return QVariant::fromValue(PackageType::fromData(data));}


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
