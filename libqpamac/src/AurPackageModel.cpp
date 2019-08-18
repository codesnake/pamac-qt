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
    auto package = m_packageList[index.row()].value<AURPackage>();
    switch (role) {
    case AurPackageModel::NameRole:
        return package.name();
    case AurPackageModel::DescriptionRole:
        return package.desc();
    case AurPackageModel::VersionRole:
        return package.version();
    case AurPackageModel::PopularityRole:
        return package.popularity();
    case AurPackageModel::InstalledVersionRole:
        return package.installedVersion();

    default:
        return QVariant::Invalid;
    }
}

void LibQPamac::AurPackageModel::sort(int column, Qt::SortOrder order){
    beginResetModel();
    std::function<bool(QVariant&,QVariant&)> sortingFunction;
    switch (column) {
    case 0:
        sortingFunction = [order](QVariant &v1, QVariant &v2)->bool{
            auto p1 = v1.value<AURPackage>();
            auto p2 = v2.value<AURPackage>();

            if(order == Qt::AscendingOrder){
                return (!p2.installedVersion().isEmpty() && p1.installedVersion().isEmpty()) ||
                        ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()>p2.name()));
            }

            return (!p1.installedVersion().isEmpty() && p2.installedVersion().isEmpty()) ||
                    ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()<p2.name()));

        };
        break;
    case 1:
        sortingFunction = [order]( QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<AURPackage>();
            auto p2 = v2.value<AURPackage>();

            if(order == Qt::AscendingOrder){
                return (p1.name()>p2.name());
            }
            return (p1.name()<p2.name());
        };
        break;
    case 2:
        sortingFunction = [order]( QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<AURPackage>();
            auto p2 = v2.value<AURPackage>();

            if(order == Qt::AscendingOrder){
                return (p1.version()>p2.version()) ||
                        ((p1.version()==p2.version()) && (p1.name()>p2.name()));
            }
            return (p1.version()<p2.version()) ||
                    ((p1.version()==p2.version()) && (p1.name()<p2.name()));
        };

        break;
    case 3:
        sortingFunction = [order](QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<AURPackage>();
            auto p2 = v2.value<AURPackage>();

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
