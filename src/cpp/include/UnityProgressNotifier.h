#ifndef UNITYPROGRESSNOTIFIER_H
#define UNITYPROGRESSNOTIFIER_H

#include <QObject>
#include "AbstractNotifier.h"
class UnityProgressNotifier:public QObject,public AbstractNotifier
{
    Q_OBJECT
public:
    UnityProgressNotifier(QObject *parent = nullptr);

    // AbstractNotifier interface
public slots:
    void setVisible(bool visible) override;
    void setProgress(double progress) override;
    void setMessage(const QString &message) override;
};

#endif // UNITYPROGRESSNOTIFIER_H
