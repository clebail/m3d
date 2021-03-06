//-----------------------------------------------------------------------------------------------
#include <QDir>
#include <QtDebug>
#include <QStringList>
#include <QMessageBox>
#include <QKeyEvent>
#include <QScrollBar>
#include "CMainWindow.h"
#include "CChooseColorDialog.h"
#include "C3dPreview.h"
//-----------------------------------------------------------------------------------------------
#define MAX_POINT_X                     200
#define MAX_POINT_Y                     200
//-----------------------------------------------------------------------------------------------
CMainWindow::CMainWindow(QString projet, QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    installEventFilter(this);

    this->projet = projet;
    map = new QHash<QString, QList<QList<SPoint *>*>*>();

    sbArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sbArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    sbArea->widget()->setMinimumWidth(MAX_POINT_X * STEPX);
    sbArea->widget()->setMinimumHeight(MAX_POINT_Y * STEPY);
    sbArea->widget()->setMaximumWidth(MAX_POINT_X * STEPX);
    sbArea->widget()->setMaximumHeight(MAX_POINT_Y * STEPY);

    loadLayers();
}
//-----------------------------------------------------------------------------------------------
CMainWindow::~CMainWindow(void) {
    clearLayers();

    delete map;
}
//-----------------------------------------------------------------------------------------------
bool CMainWindow::eventFilter(QObject *object, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(keyEvent->key() == Qt::Key_Delete) {
            if(keyEvent->modifiers() & Qt::ControlModifier) {
                dropColorPoints();
            } else {
                on_pbDropPoint_clicked();
            }

            return true;
        } else if(keyEvent->key() == Qt::Key_F1) {
            on_pbAddPoint_clicked();

            return true;
        } else if(keyEvent->key() == Qt::Key_F2) {
            editWidget->remplir();

            return true;
        }
    }

    return QObject::eventFilter(object, event);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::loadLayers(void) {
    QDir dir(TXTS_FOLDER+projet+"/");
	QStringList filters;

    filters << "*.txt";
	dir.setNameFilters(filters);
    QStringList list = dir.entryList(QDir::Files, QDir::Name);

    for(int i=0;i<list.size();i++) {
        QString layer = list.at(i);

        layer = layer.left(layer.length()-4);
        loadLayer(layer);

        layerList->addItem(layer);
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::loadLayer(QString layerName, bool force) {
    if(!map->contains(layerName) || force) {
        QString txtFileName = TXTS_FOLDER+projet+"/"+layerName+".txt";
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
                point->coul = fields[2] == "10" ? "2" : fields[2];

                if(id != oldId) {
                    list = new QList<SPoint *>();
                    mainList->append(list);
                }

                list->append(point);

                oldId = id;
            }

            txtFile.close();
            map->insert(layerName, mainList);
        } else {
			qDebug() << "unable to open file" << txtFileName;
		}
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::showLayer(QString layerName) {
    QString lDessus = QString::number(layerName.toInt()-1).rightJustified(4, '0');
    QPoint center;
    double ax, ay, bx, by;

    editWidget->setMaps(map->value(layerName), map->value(lDessus));
    getMapCenter(map->value(layerName), center);

    bx = 100.0 * STEPX;
    by = 100.0 * STEPY;

    ax = sbArea->horizontalScrollBar()->maximum() / (static_cast<double>(MAX_POINT_X) * STEPX);
    ay = sbArea->verticalScrollBar()->maximum() / (static_cast<double>(MAX_POINT_Y) * STEPY);

    sbArea->horizontalScrollBar()->setValue(static_cast<int>(center.x() * ax + bx - sbArea->size().width() / 2));
    sbArea->verticalScrollBar()->setValue(static_cast<int>(center.y() * ay + by - sbArea->size().height() / 2));
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::saveLayer(QString layer, QString fileName) {
    if(map->contains(layer)) {
        QFile txtFile(fileName);

        if(txtFile.open(QIODevice::WriteOnly)) {
            QList<QList<SPoint *>*> *mainList = map->value(layer);
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
void CMainWindow::clearLayers(void) {
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

        map->remove(i.key());

        delete list;
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::getMapCenter(QList<QList<SPoint *>*> *list, QPoint &center) {
    int minX = 0, minY = 0, maxX = 0, maxY = 0;
    bool first = true;

    for(int i=0;i<list->size();i++) {
        QList<SPoint *> *l = list->at(i);

        for(int j=0;j<l->size();j++) {
            SPoint *p = l->at(j);

            if(first || p->x < minX) {
                minX = p->x;
            }

            if(first || p->y < minY) {
                minY = p->y;
            }

            if(first || p->x > maxX) {
                maxX = p->x;
            }

            if(first || p->y > maxY) {
                maxY = p->y;
            }

            first = false;
        }
    }

    center.setX(((minX + maxX) / 2) * 1);
    center.setY(((minY + maxY) / 2) * 1);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::dropColorPoints(void) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir supprimer les points contigues ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        editWidget->dropCurrentPoint(true);
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_layerList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *) {
    QString lDessus = QString::number(current->text().toInt()-1).rightJustified(4, '0');

    loadLayer(current->text());
    loadLayer(lDessus);

    showLayer(current->text());
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbSave_clicked(bool) {
    QString layerName = layerList->currentItem()->text();
    QString txtFileName = TXTS_FOLDER+projet+"/"+layerName+".txt";

    saveLayer(layerName, txtFileName);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbAdd_clicked(bool) {
    QString nextLayer = QString::number(layerList->item(layerList->count()-1)->text().toInt()+1).rightJustified(4, '0');
    QString fileName = TXTS_FOLDER+projet+"/"+nextLayer+".txt";

    map->insert(nextLayer, new QList<QList<SPoint *>*>());

    saveLayer(nextLayer, fileName);

    clearLayers();
    loadLayers();
    loadLayer(layerList->item(layerList->count()-1)->text(), true);
    layerList->setCurrentRow(layerList->count()-1);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbSupprimer_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir supprimer cette couche ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        int currentIndex = layerList->currentRow();
        int i;

        for(i=currentIndex+1;i<layerList->count();i++) {
           QString fileName = TXTS_FOLDER+projet+"/"+layerList->item(i-1)->text()+".txt";

           loadLayer(layerList->item(i)->text());
           saveLayer(layerList->item(i)->text(), fileName);
        }

        QFile f(TXTS_FOLDER+projet+"/"+layerList->item(layerList->count()-1)->text()+".txt");
        f.remove();

        clearLayers();
        loadLayers();
        loadLayer(layerList->item(0)->text(), true);
        layerList->setCurrentRow(0);
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
void CMainWindow::on_pbRemplir_clicked(bool) {
    editWidget->remplir();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbZapLigne_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir supprimer cette ligne ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        editWidget->zapLigne();
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbAddLigne_clicked(bool) {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir ajouter cette ligne ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        editWidget->addLigne();
    }
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbPlusX_clicked(bool) {
    editWidget->plusX();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbMoinsX_clicked(bool) {
    editWidget->moinsX();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbPlusY_clicked(bool) {
    editWidget->plusY();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbMoinsY_clicked(bool) {
    editWidget->moinsY();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbSimple_clicked(bool) {
    editWidget->simplify();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_chkInverse_clicked(bool checked) {
    editWidget->setShowInverse(checked);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_chkDessus_clicked(bool checked) {
    editWidget->setShowDessus(checked);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pb3d_clicked(bool) {
    C3dPreview preview(this, map);

    preview.exec();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbDouble_clicked(bool) {
    editWidget->dedouble();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_sbTaillePoint_valueChanged(int i) {
    editWidget->setSteps(i, i);
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_chkShowLigne_clicked(bool) {
    editWidget->setShows(chkShowLigne->isChecked(), chkShowColonne->isChecked());
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_chkShowColonne_clicked(bool) {
    editWidget->setShows(chkShowLigne->isChecked(), chkShowColonne->isChecked());
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbDiffX_clicked(bool) {
    editWidget->diffX();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbDiffY_clicked(bool) {
    editWidget->diffY();
}
//-----------------------------------------------------------------------------------------------
void CMainWindow::on_pbClearDiff_clicked(bool ) {
    editWidget->clearDiffs();
}
//-----------------------------------------------------------------------------------------------
