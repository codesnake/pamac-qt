#include "AlpmPackagesModel.h"
QString prettySize(ulong bytes)
{
    std::string suffixes[7];
    suffixes[0] = "B";
    suffixes[1] = "KB";
    suffixes[2] = "MB";
    suffixes[3] = "GB";
    suffixes[4] = "TB";
    suffixes[5] = "PB";
    suffixes[6] = "EB";
    uint s = 0;
    double count = bytes;
    while (count >= 1024 && s < 7)
    {
        s++;
        count /= 1024;
    }
    return QString("%1 %2").arg(QString::number(count,'f',1)).arg(QString::fromStdString(suffixes[s]));
}
QVariant PackageModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case PackageModel::NameRole:
        return m_packageList[index.row()].name();
    case PackageModel::DescriptionRole:
        return m_packageList[index.row()].desc();
    case PackageModel::VersionRole:
        return m_packageList[index.row()].version();
    case PackageModel::SizeRole:
        return prettySize(m_packageList[index.row()].size());
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
