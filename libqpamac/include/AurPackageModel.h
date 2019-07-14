#pragma once
#include "AurPackage.h"
#include "PackageList.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QJSValue>
namespace LibQPamac {
class AurPackageModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(AURPackageList packageList READ packageList WRITE setPackageList NOTIFY packageListChanged)
    Q_PROPERTY(int columnCount READ columnCount CONSTANT)


public:
    enum PackageRole {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        VersionRole,
        PopularityRole,
        InstalledVersionRole,
    };

    AurPackageModel(AURPackageList& packageList, QObject *parent = Q_NULLPTR):QAbstractTableModel (parent),m_packageList(packageList){}
    AurPackageModel(QObject *parent = Q_NULLPTR):QAbstractTableModel(parent){}

    QHash<int, QByteArray>  roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override{ Q_UNUSED(parent) return int(m_packageList.size());}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return 3;
    }
    inline AURPackageList packageList() const
    {
        return m_packageList;
    }

    Q_INVOKABLE void sort(int column,Qt::SortOrder order = Qt::AscendingOrder) override;

public Q_SLOTS:
    inline void setPackageList(AURPackageList packageList)
    {
        beginResetModel();
        m_packageList = std::move(packageList);
        sort(3,Qt::AscendingOrder);
        endResetModel();
        Q_EMIT packageListChanged(m_packageList);
    }

Q_SIGNALS:

    void packageListChanged(AURPackageList packageList);

private:
    QStringList list = {"Name","Version","State"};
    QList<QVariant> sizeList = {"fill",40,70};
    AURPackageList m_packageList;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
} //namespace LibQPamac
