#pragma once
#include "PackageList.h"
#include "AURPackage.h"
#include <QObject>
#include <QAbstractTableModel>
#include <QJSValue>
namespace PamacQt {
class AURPackageModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(AURPackageList packageList READ packageList WRITE setPackageList NOTIFY packageListChanged)

public:
    enum PackageRole {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        VersionRole,
        PopularityRole,
        InstalledVersionRole,
    };

    AURPackageModel(AURPackageList& packageList, QObject *parent = Q_NULLPTR):QAbstractTableModel (parent),m_packageList(packageList){}
    AURPackageModel(QObject *parent = Q_NULLPTR):QAbstractTableModel(parent){}

    QHash<int, QByteArray>  roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override{ Q_UNUSED(parent);return int(m_packageList.size());}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return 4;
    }
    inline AURPackageList packageList() const
    {
        return m_packageList;
    }

    Q_INVOKABLE void sort(int column,Qt::SortOrder order = Qt::AscendingOrder) override;

public slots:
    inline void setPackageList(AURPackageList packageList)
    {
        beginResetModel();
        m_packageList = std::move(packageList);
        sort(1,Qt::DescendingOrder);
        endResetModel();
        emit packageListChanged(m_packageList);
    }

signals:

    void packageListChanged(AURPackageList packageList);

private:
    AURPackageList m_packageList;
};
} //namespace PamacQt
