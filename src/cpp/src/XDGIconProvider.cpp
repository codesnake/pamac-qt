#include "XDGIconProvider.h"
#include <QIcon>
#include <QDebug>

XDGIconProvider::XDGIconProvider():QQuickImageProvider (QQuickImageProvider::Pixmap)
{

}

QPixmap XDGIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QIcon ico;
    if(id.startsWith("builtin/")){
        QString theme = QIcon::themeName();
        QIcon::setThemeName("builtin");

        ico = QIcon::fromTheme(id.right(id.length()-8));

        QIcon::setThemeName(theme);
    } else {
        ico = QIcon::fromTheme(id);
    }
    QPixmap pm;
    if(!requestedSize.isValid()){
        pm =ico.pixmap(264);
    } else{
        pm = ico.pixmap(requestedSize);
    }
    *size = pm.size();
    return pm;
}
