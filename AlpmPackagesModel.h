#ifndef ALPMPACKAGESMODEL_H
#define ALPMPACKAGESMODEL_H
#include "AlpmList.h"
#include <QObject>
#include <QAbstractListModel>


class PackageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(AlpmPackageList packageList READ packageList WRITE setPackageList NOTIFY packageListChanged)
public:
    enum PackageRole {
        NameRole = Qt::UserRole + 1,
        SizeRole,
        DescriptionRole,
        VersionRole,
        RepoNameRole,
        IconRole,
        InstalledVersionRole,
        AppNameRole
    };

    PackageModel(AlpmPackageList packageList, QObject *parent = Q_NULLPTR):QAbstractListModel (parent),m_packageList(packageList){}
    PackageModel(QObject *parent = Q_NULLPTR):QAbstractListModel(parent){}

    QHash<int, QByteArray> roleNames() const {
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
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const{ Q_UNUSED(parent);return int(m_packageList.size());}
    AlpmPackageList packageList() const
    {
        return m_packageList;
    }

public slots:
    void setPackageList(AlpmPackageList packageList)
    {
        beginResetModel();
        m_packageList = packageList;
        endResetModel();
    }

signals:
    void packageListChanged(AlpmPackageList packageList);

private:
    AlpmPackageList m_packageList;
};
#endif // ALPMPACKAGESMODEL_H
