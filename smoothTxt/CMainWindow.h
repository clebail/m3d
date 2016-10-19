//-----------------------------------------------------------------------------------------------
#ifndef __CMAINWINDOW_H__
#define __CMAINWINDOW_H__
//-----------------------------------------------------------------------------------------------
#include <QMainWindow>
#include <QHash>
#include <QList>
#include <common.h>
#include "ui_CMainWindow.h"
//-----------------------------------------------------------------------------------------------
class CMainWindow : public QMainWindow, private Ui::CMainWindow {
        Q_OBJECT
    public:
        CMainWindow(QWidget *parent=0);
        ~CMainWindow(void);
    private:
        QHash<QString, QList<QList<SPoint *>*>*> *map;
        void loadLayers(void);
        void loadLayer(QString layerName, bool force = false);
        void showLayer(QString layerName);
    private slots:
        void on_layerList_itemClicked(QListWidgetItem *item);
        void on_pbSupprimer_clicked(bool checked = false);
        void on_pbReset_clicked(bool checked = false);
        void on_editWidget_mouseMove(int x, int y);
};
//-----------------------------------------------------------------------------------------------
#endif // __CMAINWINDOW_H__
//-----------------------------------------------------------------------------------------------
