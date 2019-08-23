#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>

class Notifier : public QObject
{
    Q_OBJECT
public:
    explicit Notifier(QString  iconName = QString(), QObject *parent = nullptr);

public Q_SLOTS:
    Q_INVOKABLE void send(const QVariantMap &options);

private:
    QString m_iconName;
};

#endif // NOTIFIER_H
