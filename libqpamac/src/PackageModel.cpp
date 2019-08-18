#include "PackageModel.h"
#include <QLocale>
#include <QModelIndex>
QHash<int, QByteArray> LibQPamac::PackageModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[DescriptionRole] = "desc";
    roles[VersionRole] = "version";
    roles[RepoNameRole] = "repo";
    roles[IconRole] = "iconUrl";
    roles[InstalledVersionRole] = "installedVersion";
    roles[AppNameRole] = "appName";
    return roles;
}

QVariant LibQPamac::PackageModel::data(const QModelIndex &index, int role) const
{
    auto package = m_packageList[index.row()].value<LibQPamac::Package>();

    switch (role) {
    case PackageModel::NameRole:
        return package.name();
    case PackageModel::DescriptionRole:
        return package.desc();
    case PackageModel::VersionRole:
        return package.version();
    case PackageModel::SizeRole:
        return QLocale().formattedDataSize(package.size());
    case PackageModel::RepoNameRole:
        return package.repo();
    case PackageModel::IconRole:
        return package.iconUrl();
    case PackageModel::InstalledVersionRole:
        return package.installedVersion();
    case PackageModel::AppNameRole:
        return package.appName();
    default:
        return QVariant::Invalid;
    }
}

void LibQPamac::PackageModel::sort(int column, Qt::SortOrder order){
    beginResetModel();
    std::function<bool(QVariant&,QVariant&)> sortingFunction;
    switch (column) {
    case 0:
        sortingFunction = [order](QVariant &v1,QVariant &v2)->bool{

            auto p1 = v1.value<Package>();
            auto p2 = v2.value<Package>();
            if(order == Qt::AscendingOrder){
                return (!p2.installedVersion().isEmpty() && p1.installedVersion().isEmpty()) ||
                        ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()>p2.name()));
            }

            return (!p1.installedVersion().isEmpty() && p2.installedVersion().isEmpty()) ||
                    ((p1.installedVersion().isEmpty() == p2.installedVersion().isEmpty()) && (p1.name()<p2.name()));

        };
        break;
    case 1:
        sortingFunction = [order](QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<Package>();
            auto p2 = v2.value<Package>();
            if(order == Qt::AscendingOrder){
                return (p1.name()>p2.name());
            }
            return (p1.name()<p2.name());
        };
        break;
    case 2:
        sortingFunction = [order](QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<Package>();
            auto p2 = v2.value<Package>();
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
            auto p1 = v1.value<Package>();
            auto p2 = v2.value<Package>();
            if(order == Qt::AscendingOrder){
                return (p1.repo()>p2.repo()) ||
                        ((p1.repo()==p2.repo()) && (p1.name()>p2.name()));
            }
            return (p1.repo()<p2.repo()) ||
                    ((p1.repo()==p2.repo()) && (p1.name()<p2.name()));
        };
        break;
    case 4:
        sortingFunction = [order](QVariant &v1,QVariant &v2)->bool{
            auto p1 = v1.value<Package>();
            auto p2 = v2.value<Package>();
            if(order == Qt::AscendingOrder){
                return (p1.size()>p2.size()) ||
                        ((p1.size()==p2.size()) && (p1.name()>p2.name()));
            }
            return (p1.size()<p2.size()) ||
                    ((p1.size()==p2.size()) && (p1.name()<p2.name()));
        };
        break;
    }
    std::sort(m_packageList.begin(),m_packageList.end(),sortingFunction);

    endResetModel();
}


QVariant LibQPamac::PackageModel::headerData(int section, Qt::Orientation orientation, int role) const
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
