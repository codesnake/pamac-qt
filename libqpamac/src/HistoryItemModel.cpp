#include "HistoryItemModel.h"
#include <QRegularExpression>
namespace LibQPamac {

HistoryItemModel::HistoryItemModel(QObject *parent) : QAbstractTableModel(parent)
{

}

QList<QVariant> HistoryItemModel::historyList() const
{
    return m_historyList;
}

void HistoryItemModel::setHistoryList(const QList<QVariant> &historyList)
{
    beginResetModel();
    m_historyList = historyList;
    endResetModel();
    Q_EMIT historyListChanged(m_historyList);
}

QList<QVariant> HistoryItem::fromStringList(const QStringList &list){
    QList<QVariant> result;
    const QRegularExpression exp("\\[(.+)\\] \\[ALPM\\] (\\S+) (\\S+) \\((.+)\\)");
    for(const QString& el:list){
        QRegularExpressionMatch match;
        if((match = exp.match(el)).hasMatch()){
            HistoryItem item;
            item.time = QDateTime::fromString(match.captured(1),Qt::ISODate);
            item.m_type = typeFromString(match.captured(2));
            item.name = match.captured(3);
            item.version = match.captured(4);

            result.append(QVariant::fromValue(item));
        }
    }
    return result;
}


QVariant HistoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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


void HistoryItemModel::sort(int column, Qt::SortOrder order)
{
    auto sortFunction = std::function<bool(QVariant&,QVariant&)>();

    switch(column){
    case 2:
        sortFunction = [order](QVariant& v1,QVariant& v2)->bool{
            auto i1 = v1.value<HistoryItem>();
            auto i2 = v2.value<HistoryItem>();
            if(order == Qt::AscendingOrder){
                return (i1.time>i2.time);
            }
            return (i1.time<i2.time);
        };
        break;
    case 1:
        sortFunction = [order](QVariant& v1,QVariant& v2)->bool{
            auto i1 = v1.value<HistoryItem>();
            auto i2 = v2.value<HistoryItem>();
            if(order == Qt::AscendingOrder){
                return (i1.type()>i2.type());
            }
            return (i1.type()<i2.type());
        };
        break;
    case 0:
        sortFunction = [order](QVariant& v1,QVariant& v2)->bool{
            auto i1 = v1.value<HistoryItem>();
            auto i2 = v2.value<HistoryItem>();
            if(order == Qt::AscendingOrder){
                return (i1.name>i2.name);
            }
            return (i1.name<i2.name);
        };
        break;
    case 3:
        sortFunction = [order](QVariant& v1,QVariant& v2)->bool{
            auto i1 = v1.value<HistoryItem>();
            auto i2 = v2.value<HistoryItem>();
            if(order == Qt::AscendingOrder){
                return (i1.version>i2.version);
            }
            return (i1.version<i2.version);
        };
        break;
    }
    beginResetModel();
    std::sort(m_historyList.begin(),m_historyList.end(),sortFunction);
    endResetModel();
}
} //namespace LibQPamac
