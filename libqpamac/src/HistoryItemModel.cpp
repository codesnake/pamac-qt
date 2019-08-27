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

} //namespace LibQPamac


QVariant LibQPamac::HistoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
