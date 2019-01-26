#pragma once
#include "PackageList.h"
#include "RepoPackage.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QJSValue>
namespace PamacQt {
class PackageModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(RepoPackageList packageList READ packageList WRITE setPackageList NOTIFY packageListChanged)

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

    PackageModel(RepoPackageList& packageList, QObject *parent = Q_NULLPTR):QAbstractTableModel (parent),m_packageList(packageList){}
    PackageModel(QObject *parent = Q_NULLPTR):QAbstractTableModel(parent){}

    QHash<int, QByteArray>  roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override{ Q_UNUSED(parent);return int(m_packageList.size());}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return 5;
    }
    inline RepoPackageList packageList() const
    {
        return m_packageList;
    }

    Q_INVOKABLE void sort(int column,Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    inline void setPackageList(RepoPackageList packageList)
    {
        beginResetModel();
        m_packageList = std::move(packageList);
        sort(1,Qt::DescendingOrder);
        endResetModel();
        emit packageListChanged(m_packageList);
    }

signals:

    void packageListChanged(RepoPackageList packageList);

private:
    RepoPackageList m_packageList;
};
} //namespace PamacQt
