#pragma once

#include <QtQuick/QQuickItem>

class DesktopProgress: public QQuickItem{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setShow NOTIFY showChanged)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(bool indeterminate READ indeterminate WRITE setIndeterminate NOTIFY indeterminateChanged)
    Q_PROPERTY(QString infoMessage READ infoMessage WRITE setInfoMessage NOTIFY infoMessageChanged)

public:
    DesktopProgress(QQuickItem* parent = nullptr);
    bool visible() const;
    double progress() const;

    bool indeterminate() const;

    QString infoMessage() const
    {
        return m_infoMessage;
    }

public Q_SLOTS:
    void setShow(bool visible);
    void setProgress(double progress);

    void setIndeterminate(bool indeterminate);

    void setInfoMessage(const QString &infoMessage);

Q_SIGNALS:
    void showChanged(bool visible);
    void progressChanged(double progress);
    void indeterminateChanged(bool indeterminate);

    void infoMessageChanged(QString infoMessage);

private:
    QString m_kjob;

    bool m_show;

    double m_progress;

    bool m_indeterminate;
    QString m_infoMessage;
};


