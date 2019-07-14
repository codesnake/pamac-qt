#include "AurPackageModel.h"
#include <QLocale>
#include <QModelIndex>
QHash<int, QByteArray> LibQPamac::AurPackageModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescriptionRole] = "desc";
    roles[VersionRole] = "version";
    roles[PopularityRole] = "popularity";
    roles[InstalledVersionRole] = "installedVersion";
    return roles;
}

QVariant LibQPamac::AurPackageModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case AurPackageModel::NameRole:
        return m_packageList[index.row()].name();
    case AurPackageModel::DescriptionRole:
        return m_packageList[index.row()].desc();
    case AurPackageModel::VersionRole:
        return m_packageList[index.row()].version();
    case AurPackageModel::PopularityRole:
        return m_packageList[index.row()].popularity();
    case AurPackageModel::InstalledVersionRole:
        return m_packageList[index.row()].installedVersion();

    default:
        return QVariant::Invalid;
    }
}

void LibQPamac::AurPackageModel::sort(int column, Qt::SortOrder order){
    beginResetModel();
    std::function<bool(AURPackage&,AURPackage&)> sortingFunction;
    switch (column) {
    case 0:
        sortingFunction = [order](AURPackage &p1, AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (!p2.installedVersion().isEmpty() && p1.installedVersion().isEmpty()) ||
                        ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()>p2.name()));
            }

            return (!p1.installedVersion().isEmpty() && p2.installedVersion().isEmpty()) ||
                    ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()<p2.name()));

        };
        break;
    case 1:
        sortingFunction = [order]( AURPackage &p1,AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (p1.name()>p2.name());
            }
            return (p1.name()<p2.name());
        };
        break;
    case 2:
        sortingFunction = [order]( AURPackage &p1,AURPackage &p2)->bool{
            if(order == Qt::AscendingOrder){
                return (p1.version()>p2.version()) ||
                        ((p1.version()==p2.version()) && (p1.name()>p2.name()));
            }
            return (p1.version()<p2.version()) ||
                    ((p1.version()==p2.version()) && (p1.name()<p2.name()));
        };

        break;
    case 3:
        sortingFunction = [order](AURPackage &p1,AURPackage &p2)->bool{
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


QVariant LibQPamac::AurPackageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(section>=columnCount()) {
        return QVariant::Invalid;
    }

    switch (role) {
    case Qt::DisplayRole:
        return list[section];

    case Qt::SizeHintRole:
        return sizeList[section];

    }
    return QVariant::Invalid;
}
