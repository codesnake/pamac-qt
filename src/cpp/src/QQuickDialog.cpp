#include "QQuickDialog.h"

QQuickDialog::QQuickDialog(const QUrl &url, QWidget* parent):QDialog(parent),
    nestedWidget(url,this)
{
    QObject::connect(nestedWidget.rootObject(), SIGNAL(accepted()),
                     this, SLOT(accept()));
    QObject::connect(nestedWidget.rootObject(),SIGNAL(rejected()),
                     this,SLOT(reject()));
}

void QQuickDialog::showEvent(QShowEvent *)
{
    resize(600,400);
    setFixedSize(size());
    QMetaObject::invokeMethod(nestedWidget.rootObject(), "onShow");
}
