#include <QApplication>
#include <Application.h>
#include <QtDBus>
static QQmlDebuggingEnabler enabler;


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    return Application().exec();
}
