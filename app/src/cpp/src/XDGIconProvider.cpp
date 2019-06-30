#include "XDGIconProvider.h"
#include <QIcon>
#include <QDebug>
#include <QDirIterator>

XDGIconProvider::XDGIconProvider():QQuickImageProvider (QQuickImageProvider::Pixmap)
{

}

QPixmap XDGIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QIcon ico;
    if(id.startsWith("builtin/")){

        ico = QIcon(":/icons/builtin/16x16/status/"+id.right(id.length()-8)+".svg");

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
