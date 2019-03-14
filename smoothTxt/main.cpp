//-----------------------------------------------------------------------------------------------
#include <QTranslator>
#include <QTextCodec>
#include <QApplication>
#include <iostream>
#include "CMainWindow.h"
//-----------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QTranslator translator;
    int ret;
    CMainWindow *mainWindow;

    if(argc != 2) {
        std::cout << "Usage: smouthTxt <projet name>" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);

    translator.load("qt_fr", app.applicationDirPath());
    app.installTranslator(&translator);

    mainWindow = new CMainWindow(QString(argv[1]));
    mainWindow->showMaximized();

    ret=app.exec();

    delete mainWindow;

    return ret;
}
//-----------------------------------------------------------------------------------------------
