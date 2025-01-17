#include "mainwindow.h"
#include "libmgr.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    sqlite3* DB;
    char* messageError;
    initDB(DB, messageError);


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MyLibraryManagerQt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    w.setWindowTitle("Full-Screen App");

    w.showMaximized();

    w.show();

    return a.exec();
}
