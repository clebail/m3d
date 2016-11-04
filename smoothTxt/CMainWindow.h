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
        virtual bool eventFilter(QObject *object, QEvent *event);
    private:
        QHash<QString, QList<QList<SPoint *>*>*> *map;
        void loadLayers(void);
        void loadLayer(QString layerName, bool force = false);
        void showLayer(QString layerName);
        void saveLayer(QString layer, QString fileName);
        void clearLayers(void);
    private slots:
        void on_layerList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
        void on_pbSave_clicked(bool checked = false);
        void on_pbAdd_clicked(bool checked = false);
        void on_pbSupprimer_clicked(bool checked = false);
        void on_pbReset_clicked(bool checked = false);
        void on_editWidget_mouseMove(int x, int y);
        void on_pbDropPoint_clicked(bool checked = false);
        void on_pbAddPoint_clicked(bool checked = false);
        void on_pbAddGroupe_clicked(bool checked = false);
        void on_pbColor_clicked(bool checked = false);
        void on_pbRemplir_clicked(bool checked = false);
        void on_pbZapLigne_clicked(bool checked = false);
        void on_pbAddLigne_clicked(bool checked = false);
        void on_pbPlusX_clicked(bool checked = false);
        void on_pbMoinsX_clicked(bool checked = false);
        void on_pbPlusY_clicked(bool checked = false);
        void on_pbMoinsY_clicked(bool checked = false);
        void on_pbSimple_clicked(bool checked = false);
};
//-----------------------------------------------------------------------------------------------
#endif // __CMAINWINDOW_H__
//-----------------------------------------------------------------------------------------------
