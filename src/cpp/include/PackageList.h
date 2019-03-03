#pragma once
#include <pamac.h>
#include "RepoPackage.h"
#include "AURPackage.h"
#include <QObject>
#include <QVector>
#include <QDebug>
#include <memory>

namespace PamacQt {


class RepoPackageList
{
    Q_GADGET
    Q_PROPERTY(int length READ size CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
public:
RepoPackageList() = default;
    RepoPackageList(GList* list){

        g_list_foreach(list,[](void* data,void* resultPtr){
            auto result = reinterpret_cast<RepoPackageList*>(resultPtr);
            result->m_list->append(RepoPackage(static_cast<PamacPackage*>(data)));
        },this);

    }
    Q_INVOKABLE inline RepoPackage operator[](int index) const
    {
        return this->at(index);
    }
    Q_INVOKABLE RepoPackage at(int index) const
    {
        return m_list->at(index);
    }
Q_INVOKABLE QString packageType(){
        return "Repo";
    }
    static RepoPackageList fromGList(GList* list,GDestroyNotify freeFunc = nullptr)
    {
        RepoPackageList result;

        g_list_foreach(list,[](void* data,void* resultPtr){
            auto result = reinterpret_cast<RepoPackageList*>(resultPtr);
            result->m_list->append(RepoPackage(static_cast<PamacPackage*>(data)));
        },&result);

        return result;
    }

    Q_INVOKABLE inline int size() const{return m_list->length();}


    QList<RepoPackage>::iterator begin(){
        return m_list->begin();
    }
    QList<RepoPackage>::iterator end(){
        return m_list->end();
    }

    void append(const RepoPackage& pkg){
        m_list->append(pkg);
    }





private:

    std::shared_ptr<QList<RepoPackage>> m_list = std::make_shared<QList<RepoPackage>>();
};

class AURPackageList
{
    Q_GADGET
    Q_PROPERTY(int length READ size CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
public:

    AURPackageList() = default;
    Q_INVOKABLE inline AURPackage operator[](int index) const
    {
        return this->at(index);
    }
    Q_INVOKABLE AURPackage at(int index) const
    {
        return m_list[index];
    }
    Q_INVOKABLE QString packageType(){
            return "AUR";
        }
    static  AURPackageList fromGList(GList* list,GDestroyNotify freeFunc = nullptr)
    {
        AURPackageList result;

        g_list_foreach(list,[](void* data,void* resultPtr){
            auto result = reinterpret_cast<AURPackageList*>(resultPtr);
            result->m_list.append(static_cast<PamacAURPackage*>(data));
        },&result);

        return result;
    }
    Q_INVOKABLE inline int size() const{return m_list.length();}


    QList<AURPackage>::iterator begin(){
        return m_list.begin();
    }
    QList<AURPackage>::iterator end(){
        return m_list.end();
    }

    void append(const AURPackage& pkg){
        m_list.append(pkg);
    }





private:

    QList<AURPackage> m_list;
};


} //namespace PamacQt
Q_DECLARE_METATYPE(PamacQt::RepoPackageList)
Q_DECLARE_METATYPE(PamacQt::AURPackageList)
