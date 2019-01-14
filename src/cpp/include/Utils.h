#pragma once
#include "glib.h"
#include <QStringList>
#include <vector>
#include <QVariantList>

namespace PamacQt {
namespace Utils {

#define PAMAC_QT_STRING_PROPERTY_GET(name,method)\
    Q_PROPERTY(QString name READ name CONSTANT)\
    QString name() const\
{\
    return QString::fromUtf8(method);\
    }

#define PAMAC_QT_STRING_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(QString getName READ getName WRITE setName)\
    QString getName() const\
{\
    return QString::fromUtf8(method);\
    }\
    void setName(const QString& str)\
{\
    setMethod(str.toUtf8());\
    }

#define PAMAC_QT_URL_PROPERTY_GET(name,method)\
    Q_PROPERTY(QUrl name READ name CONSTANT)\
    QUrl name() const\
{\
    return QUrl(method);\
    }

#define PAMAC_QT_INT_PROPERTY_GET(name,method)\
    Q_PROPERTY(int name READ name CONSTANT)\
    int name() const\
{\
    return int(method);\
    }
#define PAMAC_QT_INT_PROPERTY_GET_SET(getName,getMethod,setName,setMethod)\
    Q_PROPERTY(int getName READ getName WRITE setName)\
    int getName() const\
{\
    return int(getMethod);\
}\
    void setName(int getName){\
    setMethod;\
}
#define PAMAC_QT_STRINGLIST_PROPERTY_GET(name,method)\
    Q_PROPERTY(QStringList name READ name CONSTANT)\
    QStringList name() const\
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
    bool name() const\
{\
    return method;\
    }\
    void setName(bool name)\
{\
    setMethod;\
    }


std::vector<char*> qStringListToCStringVector(const QStringList&);
QStringList cStringArrayToQStringList(char** arr,int size);
GList* qVariantListToGList(const QVariantList& lst);
GVariant* qVariantToGVariant(const QVariant&);
QStringList gListToQStringList(GList* lst,bool freeOriginal = false);
} // namespace Utils
} // namespace PamacQt
