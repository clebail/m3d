//-----------------------------------------------------------------------------------------------
#include <QTranslator>
#include <QTextCodec>
#include <iostream>
#include "CMainWindow.h"
//-----------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QTranslator translator;
    int ret;
    CMainWindow *mainWindow;

    if(argc != 2) {
        std::cout << "Usage: createBinvox <projet name>" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    translator.load("qt_fr", app.applicationDirPath());
    app.installTranslator(&translator);

    mainWindow = new CMainWindow(QString(argv[1]));
    mainWindow->installEventFilter(mainWindow);
    mainWindow->showMaximized();

    ret=app.exec();

    delete mainWindow;

    return ret;
}
//-----------------------------------------------------------------------------------------------
