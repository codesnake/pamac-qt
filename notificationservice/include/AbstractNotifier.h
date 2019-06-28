#pragma once
#include <QObject>

class AbstractNotifier
{
public:
    virtual ~AbstractNotifier() = default;
public slots:
    virtual void setVisible(bool visible) = 0;
    virtual void setProgress(double progress) = 0;
    virtual void setMessage(const QString& message) =0;

};
Q_DECLARE_INTERFACE(AbstractNotifier,"AbstractNotifier")
