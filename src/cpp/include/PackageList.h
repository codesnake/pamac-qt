#pragma once
#include <pamac.h>
#include "Package.h"
#include <QObject>
#include <QVector>
#include <QDebug>
#include <memory>
#include <iostream>

namespace PamacQt {
class PackageList
{
    Q_GADGET
    Q_PROPERTY(int length READ size CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
public:

    inline PackageList(GList* list){
        g_list_foreach(list,[](void *el,void* ptr){
            auto m_list = reinterpret_cast<QList<Package>*>(ptr);
            m_list->append(Package(reinterpret_cast<PamacPackage*>(el)));
        },&m_list);
    }
    PackageList()= default;
    Q_INVOKABLE inline Package operator[](int index) const
    {
        return this->at(index);
    }
    Q_INVOKABLE Package at(int index) const
    {
        return m_list[index];
    }

    Q_INVOKABLE inline int size() const{return m_list.length();}

private:
    QList<Package> m_list;
};
} //namespace PamacQt
Q_DECLARE_METATYPE(PamacQt::PackageList)
