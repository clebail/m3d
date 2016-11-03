//-----------------------------------------------------------------------------------------------
#include <QTranslator>
#include <QTextCodec>
#include "CMainWindow.h"
//-----------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QTranslator translator;
    int ret;
    CMainWindow *mainWindow;

    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    translator.load("qt_fr", app.applicationDirPath());
    app.installTranslator(&translator);

    mainWindow = new CMainWindow();
    mainWindow->installEventFilter(mainWindow);
    mainWindow->showMaximized();

    ret=app.exec();

    delete mainWindow;

    return ret;
}
//-----------------------------------------------------------------------------------------------
