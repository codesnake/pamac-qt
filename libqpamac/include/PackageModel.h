#pragma once
#include <AlpmPackage.h>
#include <QObject>
#include <QAbstractTableModel>
#include <QJSValue>
namespace LibQPamac {
class PackageModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QVariantList packageList READ packageList WRITE setPackageList NOTIFY packageListChanged)
    Q_PROPERTY(int columnCount READ columnCount CONSTANT)

public:
    enum PackageRole {
        NameRole = Qt::UserRole + 1,
        SizeRole,
        DescriptionRole,
        VersionRole,
        RepoNameRole,
        IconRole,
        InstalledVersionRole,
        AppNameRole,
        DownloadSizeRole
    };

    PackageModel(QList<AlpmPackage>& packageList, QObject *parent = Q_NULLPTR):QAbstractTableModel (parent),m_packageList(packageList){}
    PackageModel(QObject *parent = Q_NULLPTR):QAbstractTableModel(parent){}

    QHash<int, QByteArray>  roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override{ Q_UNUSED(parent);return int(m_packageList.size());}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return 5;
    }
    inline QVariantList packageList() const
    {
        QVariantList result;
        std::transform(m_packageList.begin(),m_packageList.end(),result.begin(),[](const AlpmPackage& value){return QVariant::fromValue(value);});
        return result;
    }

    Q_INVOKABLE void sort(int column,Qt::SortOrder order = Qt::AscendingOrder) override;

public Q_SLOTS:
    inline void setPackageList(const QVariantList& packageList)
    {
        beginResetModel();
        m_packageList.clear();
        for(auto& var : packageList){
            m_packageList<<var.value<AlpmPackage>();
        }
        sort(1,Qt::DescendingOrder);
        endResetModel();
        Q_EMIT packageListChanged(packageList);
    }

Q_SIGNALS:

    void packageListChanged(QVariantList packageList);

private:
    QStringList list = {"Name","Version","Repository","Size","State"};
    QList<QVariant> sizeList = {"fill",40,50,60,70};
    QList<AlpmPackage> m_packageList;

};
} //namespace LibQPamac
