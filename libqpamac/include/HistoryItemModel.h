#pragma once

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <QtDebug>
namespace LibQPamac {


struct HistoryItem{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString version MEMBER version)
    Q_PROPERTY(QDateTime time MEMBER time)
    Q_PROPERTY(QString type READ type WRITE setType)
public:
    enum Type{
        ALPMInstalled,
        ALPMRemoved,
        ALPMUpgraded,
        Unknown
    };
    static QString typeToString(Type t){
        switch (t) {
        case ALPMInstalled:
            return "Installed";
        case ALPMRemoved:
            return "Removed";
        case ALPMUpgraded:
            return "Upgraded";
        default:
            return "Unknown";
        }
    }
    static Type typeFromString(const QString& s){
        QString lowS = s.toLower();
        if(lowS=="installed"){
            return ALPMInstalled;
        }
        if(lowS=="removed"){
            return ALPMRemoved;
        }
        if(lowS=="upgraded"){
            return ALPMUpgraded;
        }
        return Unknown;
    }
    QString type(){
        return typeToString(m_type);
    }
    void setType(const QString& type){
        m_type = typeFromString(type);
    }
    static QList<QVariant> fromStringList(const QStringList& lst);
    QDateTime time;
    Type m_type;
    QString name;
    QString version;
};

class HistoryItemModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> historyList READ historyList WRITE setHistoryList NOTIFY historyListChanged)
    Q_PROPERTY(int columnCount READ columnCount CONSTANT)
    Q_PROPERTY(int rowCount READ rowCount CONSTANT)
public:
    explicit HistoryItemModel(QObject *parent = nullptr);

    QList<QVariant> historyList() const;
    void setHistoryList(const QList<QVariant> &historyList);

private:
    QList<QVariant> m_historyList;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return m_historyList.length();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return 4;
    }
    QVariant data(const QModelIndex &index, int role) const override
    {
        if(role==Qt::DisplayRole){
            switch (index.column()) {
            case 2:
                return m_historyList[index.row()].value<HistoryItem>().time.toString();
            case 1:
                return HistoryItem::typeToString(m_historyList[index.row()].value<HistoryItem>().m_type);
            case 0:
                return m_historyList[index.row()].value<HistoryItem>().name;
            case 3:
                return m_historyList[index.row()].value<HistoryItem>().version;
            }
        }
        return  QVariant::Invalid;
    }
Q_SIGNALS:
    void historyListChanged(const QList<QVariant>& historyList);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override
    {
        return {{Qt::DisplayRole,"modelData"}};
    }

    // QAbstractItemModel interface
public:
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private:
    QStringList list = {"Name","Action","Date","Version"};
    QList<QVariant> sizeList = {"fill","fill","fill","fill"};
};

}//namespace LibQPamac

Q_DECLARE_METATYPE(LibQPamac::HistoryItem)
