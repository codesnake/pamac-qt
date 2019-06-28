#ifndef HISTORYITEMMODEL_H
#define HISTORYITEMMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <QtDebug>
namespace LibQPamac {


struct HistoryItem{
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
    static QList<HistoryItem> fromStringList(const QStringList& lst);
    QDateTime time;
    Type type;
    QString name;
    QString version;
};

class HistoryItemModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QList<HistoryItem> historyList READ historyList WRITE setHistoryList NOTIFY historyListChanged)
public:
    explicit HistoryItemModel(QObject *parent = nullptr);

    QList<HistoryItem> historyList() const;
    void setHistoryList(const QList<HistoryItem> &historyList);

private:
    QList<HistoryItem> m_historyList;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return m_historyList.length();
    }
    int columnCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        return 4;
    }
    QVariant data(const QModelIndex &index, int role) const override
    {
        if(role==Qt::DisplayRole){
            switch (index.column()) {
            case 0:
                return m_historyList[index.row()].time.toString();
            case 1:
                return HistoryItem::typeToString(m_historyList[index.row()].type);
            case 2:
                return m_historyList[index.row()].name;
            case 3:
                return m_historyList[index.row()].version;
            }
        }
        return  QVariant::Invalid;
    }
Q_SIGNALS:
    void historyListChanged(const QList<HistoryItem>& historyList);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override
    {
        return {{Qt::DisplayRole,"modelData"}};
    }
};

}//namespace LibQPamac
#endif // HISTORYITEMMODEL_H
