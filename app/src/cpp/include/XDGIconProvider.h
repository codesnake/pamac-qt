#ifndef XDGICONPROVIDER_H
#define XDGICONPROVIDER_H
#include <QQuickImageProvider>

class XDGIconProvider:public QQuickImageProvider
{
public:
    XDGIconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;


};

#endif // XDGICONPROVIDER_H
