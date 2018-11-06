#include "PackageModel.h"
#include <QLocale>
QVariant PamacQt::PackageModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case PackageModel::NameRole:
        return m_packageList[index.row()].name();
    case PackageModel::DescriptionRole:
        return m_packageList[index.row()].desc();
    case PackageModel::VersionRole:
        return m_packageList[index.row()].version();
    case PackageModel::SizeRole:
        return QLocale().formattedDataSize(m_packageList[index.row()].size());
    case PackageModel::RepoNameRole:
        return m_packageList[index.row()].repo();
    case PackageModel::IconRole:
        return m_packageList[index.row()].iconUrl();
    case PackageModel::InstalledVersionRole:
        return m_packageList[index.row()].installedVersion();
    case PackageModel::AppNameRole:
        return m_packageList[index.row()].appName();
    default:
        return QVariant::Invalid;
    }
}
