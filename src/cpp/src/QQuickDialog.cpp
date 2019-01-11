#include "QQuickDialog.h"

QQuickDialog::QQuickDialog(const QUrl &url, QWidget* parent):QDialog(parent),
    nestedWidget(url,this)
{
    QObject::connect(nestedWidget.rootObject(), SIGNAL(dialogAccepted()),
                     this, SLOT(accept()));
    QObject::connect(nestedWidget.rootObject(),SIGNAL(dialogRejected()),
                     this,SLOT(reject()));
}

void QQuickDialog::showEvent(QShowEvent *)
{
    resize(600,400);
    setFixedSize(size());
    QMetaObject::invokeMethod(nestedWidget.rootObject(), "onShow");
}
