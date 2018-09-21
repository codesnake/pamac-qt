#ifndef ALPMLIST_H
#define ALPMLIST_H
#include <pamac.h>
#include "AlpmPkg.h"
#include <QObject>

class AlpmPackageList
{
    Q_GADGET
public:
    AlpmPackageList(GList* list):m_list(list){}
    AlpmPackageList():m_list(new GList()){}

    AlpmPkg operator[](int index) const
    {
        return AlpmPkg(static_cast<PamacPackage*>(g_list_nth(m_list,index)->data));
    }

    size_t size() const{return g_list_length(m_list);}
private:
    GList* m_list;

};
Q_DECLARE_METATYPE(AlpmPackageList)
#endif // ALPMLIST_H
