#include "mainwindow.h"
#include <QApplication>

#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString defaultNotesFile = QDir::home().absolutePath()  + QDir::separator() + QStringLiteral(".QtNotesDefault");
    MainWindow w(defaultNotesFile);
    w.show();

    return a.exec();
}
