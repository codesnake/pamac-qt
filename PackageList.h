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
public:

    PackageList(GList* list):m_list(std::shared_ptr<GList>(list,[=](GList* list){g_list_free_full (list, g_object_unref);})){}
    PackageList(){}
    Package operator[](uint index) const
    {
        return Package(static_cast<PamacPackage*>(g_list_nth_data(m_list.get(),index)));
    }

    size_t size() const{return g_list_length(m_list.get());}
private:
    //Qt model requires (a lot of) indexed access to the list elements.
    //IMO it's more preferable to copy all elements to an array structure than using a linked list
    std::shared_ptr<GList> m_list;
};
} //namespace PamacQt
Q_DECLARE_METATYPE(PamacQt::PackageList)
