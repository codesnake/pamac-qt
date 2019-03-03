#include "AURPackageModel.h"
#include <QLocale>
#include <QModelIndex>
QHash<int, QByteArray> PamacQt::AURPackageModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescriptionRole] = "desc";
    roles[VersionRole] = "version";
    roles[PopularityRole] = "popularity";
    roles[InstalledVersionRole] = "installedVersion";
    return roles;
}

QVariant PamacQt::AURPackageModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case AURPackageModel::NameRole:
        return m_packageList[index.row()].name();
    case AURPackageModel::DescriptionRole:
        return m_packageList[index.row()].desc();
    case AURPackageModel::VersionRole:
        return m_packageList[index.row()].version();
    case AURPackageModel::PopularityRole:
        return m_packageList[index.row()].popularity();
    case AURPackageModel::InstalledVersionRole:
        return m_packageList[index.row()].installedVersion();

    default:
        return QVariant::Invalid;
    }
}

void PamacQt::AURPackageModel::sort(int column, Qt::SortOrder order){
    beginResetModel();
    std::function<bool(const AURPackage&,const AURPackage&)> sortingFunction;
    switch (column) {
    case 0:
        sortingFunction = [order](const AURPackage &p1,const AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (!p2.installedVersion().isEmpty() && p1.installedVersion().isEmpty()) ||
                        ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()>p2.name()));
            }

            return (!p1.installedVersion().isEmpty() && p2.installedVersion().isEmpty()) ||
                    ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()<p2.name()));

        };
        break;
    case 1:
        sortingFunction = [order](const AURPackage &p1,const AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (p1.name()>p2.name());
            }
            return (p1.name()<p2.name());
        };
        break;
    case 2:
        sortingFunction = [order](const AURPackage &p1,const AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (p1.version()>p2.version()) ||
                        ((p1.version()==p2.version()) && (p1.name()>p2.name()));
            }
            return (p1.version()<p2.version()) ||
                    ((p1.version()==p2.version()) && (p1.name()<p2.name()));
        };

        break;
    case 3:
        sortingFunction = [order](const AURPackage &p1,const AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (p1.popularity()>p2.popularity()) ||
                        ((p1.popularity()==p2.popularity()) && (p1.name()>p2.name()));
            }
            return (p1.popularity()<p2.popularity()) ||
                    ((p1.popularity()==p2.popularity()) && (p1.name()<p2.name()));
        };
        break;
    }

    std::sort(m_packageList.begin(),m_packageList.end(),sortingFunction);

    endResetModel();
}
