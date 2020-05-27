#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator tr1,tr2;
    tr1.load(":/localizations/pngeditor_"+QLocale::system().name() );
    tr2.load(QString("qtbase_")+QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&tr1);
    a.installTranslator(&tr2);



    MainWindow w;
    w.show();
    return a.exec();
}
