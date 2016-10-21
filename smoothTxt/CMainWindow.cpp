//-----------------------------------------------------------------------------------------------
#include <QDir>
#include <QtDebug>
#include <QStringList>
#include <QMessageBox>
#include "CMainWindow.h"
#include "CChooseColorDialog.h"
//-----------------------------------------------------------------------------------------------
CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    map = new QHash<QString, QList<QList<SPoint *>*>*>();

    loadLayers();
}
//-----------------------------------------------------------------------------------------------
CMainWindow::~CMainWindow(void) {
    QHashIterator<QString, QList<QList<SPoint *>*>*> i(*map);
    while (i.hasNext()) {
        int j, k;
        QList<QList<SPoint *>*> *list;

        i.next();
        list = i.value();

        for(j=0;j<list->size();j++) {
            QList<SPoint *> *l = list->at(j);

            for(k=0;k<l->size();k++) {
                delete l->at(k);
            }

            delete l;
        }
        delete list;
    }

    delete map;
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::loadLayers(void) {
    QDir dir(TXTS_FOLDER);
    QStringList list = dir.entryList(QDir::Files, QDir::Name);

    for(int i=0;i<list.size();i++) {
        QString layer = list.at(i);

        layer = layer.left(layer.length()-4);

        layerList->addItem(layer);
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::loadLayer(QString layerName, bool force) {
    if(!map->contains(layerName) || force) {
        QString txtFileName = TXTS_FOLDER+layerName+".txt";
        QFile txtFile(txtFileName);

        if(txtFile.open(QIODevice::ReadOnly)) {
            QTextStream txtStream(&txtFile);
            int oldId = 0;
            QList<QList<SPoint *>*> *mainList = new QList<QList<SPoint *>*>();
            QList<SPoint *> *list = 0;

            while(!txtStream.atEnd()) {
                QString line = txtStream.readLine();
                QStringList fields = line.split(";");
                SPoint *point = new SPoint;
                int id = fields[3].toInt();

                point->x = fields[0].toInt();
                point->y = fields[1].toInt();
                point->coul = fields[2];

                if(id != oldId) {
                    list = new QList<SPoint *>();
                    mainList->append(list);
                }

                list->append(point);

                oldId = id;
            }

            txtFile.close();
            map->insert(layerName, mainList);
        }
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::showLayer(QString layerName) {
    editWidget->setMap(map->value(layerName));
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_layerList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *) {
    loadLayer(current->text());
    showLayer(current->text());
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbSave_clicked(bool) {
    QString layerName = layerList->currentItem()->text();
    if(map->contains(layerName)) {
        QString txtFileName = TXTS_FOLDER+layerName+".txt";
        QFile txtFile(txtFileName);

        if(txtFile.open(QIODevice::WriteOnly)) {
            QList<QList<SPoint *>*> *mainList = map->value(layerName);
            QTextStream txtStream(&txtFile);

            for(int i=0;i<mainList->size();i++) {
                QList<SPoint *> *list = mainList->at(i);

                for(int j=0;j<list->size();j++) {
                    SPoint *point = list->at(j);
                    QString carre="%1;%2;%3;%4\n";

                    txtStream << carre.arg(point->x).arg(point->y).arg(point->coul).arg(i+1);
                }
            }

            txtFile.close();
        }
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbSupprimer_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir supprimer cette couche ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        //TODO
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbReset_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir recharger cette couche ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        loadLayer(layerList->currentItem()->text(), true);
        showLayer(layerList->currentItem()->text());
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_editWidget_mouseMove(int x, int y) {
    lblPos->setText("X: "+QString::number(x)+" Y: "+QString::number(y));
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbDropPoint_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir supprimer ce point ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        editWidget->dropCurrentPoint();
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbAddPoint_clicked(bool) {
    editWidget->addPoint();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbUpPoint_clicked(bool) {
    editWidget->upPoint();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbDownPoint_clicked(bool) {
    editWidget->downPoint();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbAddGroupe_clicked(bool) {
    editWidget->addGroupe();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbColor_clicked(bool) {
    CChooseColorDialog *chooseColorDialog = new CChooseColorDialog(this);

    if(chooseColorDialog->exec() == QDialog::Accepted) {
        editWidget->setColor(chooseColorDialog->getColor());
    }

    delete chooseColorDialog;
}
//-----------------------------------------------------------------------------------------------
