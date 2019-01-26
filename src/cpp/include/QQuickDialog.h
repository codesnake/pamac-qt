#pragma once
#include <QLayout>
#include <QDialog>
#include <QQuickWidget>
#include <QQuickItem>
#include <QEventLoop>

class QQuickDialog: public QDialog
{
    Q_OBJECT
public:
    QQuickDialog(const QUrl& url,QWidget *parent = nullptr);

    Q_INVOKABLE void setQQuickWidgetProperty(const QString& name,const QVariant& value){
       nestedWidget.rootObject()->setProperty(name.toUtf8(),value);
    }
    Q_INVOKABLE QVariant qQuickWidgetProperty(const QString& name){
        return nestedWidget.rootObject()->property(name.toUtf8());
    }
private:
    QQuickWidget nestedWidget;
    void showEvent(QShowEvent*) override;
};
