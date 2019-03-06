//-----------------------------------------------------------------------------------------------
#include "C3dPreview.h"
//-----------------------------------------------------------------------------------------------
#define COEF        (10.0f)
#define UNIT        (1/COEF)
#define UNITY		(UNIT*1.23f)
#define UNIT2       (UNIT/2.0f)
#define UNITY2		(UNITY/2.0f)
#define KNOBRADIUS  (UNIT/3.25f)
#define KNOBHEIGHT  (UNIT/4.6f)
//-----------------------------------------------------------------------------------------------
C3dPreview::C3dPreview(QWidget *parent, QHash<QString, QList<QList<SPoint *>*>*> *map) : QDialog(parent) {
    setupUi(this);

    lvMenu = new QMenu(this);
    lvMenu->addAction(tr("Sélection tous"), this, SLOT(selectedAll()));
    lvMenu->addAction(tr("Annuler la sélection"), this, SLOT(selectedNone()));

    lvLayers->setSelectionMode(QAbstractItemView::MultiSelection);
    connect(lvLayers, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(popupMenu(const QPoint &)));
    connect(lvColors, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(popupMenu(const QPoint &)));

    lvColors->setSelectionMode(QAbstractItemView::MultiSelection);

    this->map = map;

    if(map != nullptr) {
        createList();
        computeMap();
        w3d->setMap(sfMap);
    }

    popupSender = nullptr;
}
//-----------------------------------------------------------------------------------------------
C3dPreview::~C3dPreview(void) {
    clearMap();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::createList(void) {
    QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);

    colors.clear();
    lvLayers->clear();
    lvColors->clear();

    while (it.hasNext()) {
        QList<QList<SPoint *>*> *list;

        it.next();
        list = it.value();
        for(int i=0;i<list->size();i++) {
            QList<SPoint *> *sl = list->at(i);

            for(int j=0;j<sl->size();j++) {
                QString color = sl->at(j)->coul;

                if(!colors.contains(color)) {
                    colors << color;
                    lvColors->addItem(getColorNameFromIdx(color)+" ("+color+")");
                }
            }
        }

        lvLayers->addItem(it.key());
    }

    lvLayers->selectAll();
    lvColors->selectAll();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::computeMap(void) {
    QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);
    QList<QListWidgetItem *> selectedItems = lvLayers->selectedItems();
    QList<QListWidgetItem *> selectedColorsItems = lvColors->selectedItems();
    QStringList selectedText;
    QStringList selectedColors;

    clearMap();

    for(int i=0;i<selectedItems.size();i++) {
        selectedText << selectedItems.at(i)->text();
    }

    for(int i=0;i<lvColors->count();i++) {
        if(lvColors->item(i)->isSelected()) {
            selectedColors << colors.at(i);
        }
    }

    while (it.hasNext()) {
        QList<QList<SPoint *>*> *list;

        it.next();
        list = it.value();

        if(selectedText.contains(it.key())) {
            for(int i=0;i<list->size();i++) {
                QList<SPoint *> *sl = list->at(i);

                for(int j=0;j<sl->size();j++) {
                    if(selectedColors.contains(sl->at(j)->coul)) {
                        float x = (sl->at(j)->x / STEPX) / COEF;;
                        float y = (map->size() / 2 - it.key().toInt() - 1) * UNITY;
                        float z = (sl->at(j)->y / STEPY) / COEF;
                        QColor color = getColor(sl->at(j)->coul);

                        GLfloat coords[6][4][3] = {
                            { { x+UNIT2, y-UNITY2, z+UNIT2 }, { x+UNIT2, y-UNITY2, z-UNIT2 }, { x+UNIT2, y+UNITY2, z-UNIT2 }, { x+UNIT2, y+UNITY2, z+UNIT2 } },
                            { { x-UNIT2, y-UNITY2, z-UNIT2 }, { x-UNIT2, y-UNITY2, z+UNIT2 }, { x-UNIT2, y+UNITY2, z+UNIT2 }, { x-UNIT2, y+UNITY2, z-UNIT2 } },
                            { { x+UNIT2, y-UNITY2, z-UNIT2 }, { x-UNIT2, y-UNITY2, z-UNIT2 }, { x-UNIT2, y+UNITY2, z-UNIT2 }, { x+UNIT2, y+UNITY2, z-UNIT2 } },
                            { { x-UNIT2, y-UNITY2, z+UNIT2 }, { x+UNIT2, y-UNITY2, z+UNIT2 }, { x+UNIT2, y+UNITY2, z+UNIT2 }, { x-UNIT2, y+UNITY2, z+UNIT2 } },
                            { { x-UNIT2, y-UNITY2, z-UNIT2 }, { x+UNIT2, y-UNITY2, z-UNIT2 }, { x+UNIT2, y-UNITY2, z+UNIT2 }, { x-UNIT2, y-UNITY2, z+UNIT2 } },
                            { { x-UNIT2, y+UNITY2, z+UNIT2 }, { x+UNIT2, y+UNITY2, z+UNIT2 }, { x+UNIT2, y+UNITY2, z-UNIT2 }, { x-UNIT2, y+UNITY2, z-UNIT2 } }
                        };

                        for(int k=0;k<6;k++) {
                            SFace *face = new SFace();

                            memcpy(&face->coords, &coords[k], sizeof(GLfloat[4][3]));
                            face->color = color;

                            sfMap.append(face);
                        }
                    }
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::clearMap(void) {
    for(int i=0;i<sfMap.size();i++) {
        delete sfMap.at(i);
    }
    sfMap.clear();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_hsX_valueChanged(int value) {
    w3d->setRotate(static_cast<float>(hsX->value()), static_cast<float>(hsY->value()), static_cast<float>(hsZ->value()));

    lbValueX->setText(QString::number(value));
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_hsY_valueChanged(int value) {
    w3d->setRotate(static_cast<float>(hsX->value()), static_cast<float>(hsY->value()), static_cast<float>(hsZ->value()));

    lbValueY->setText(QString::number(value));
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_hsZ_valueChanged(int value) {
    w3d->setRotate(static_cast<float>(hsX->value()), static_cast<float>(hsY->value()), static_cast<float>(hsZ->value()));

    lbValueZ->setText(QString::number(value));
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_lvLayers_itemSelectionChanged(void) {
    computeMap();
    w3d->setMap(sfMap);
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_lvColors_itemSelectionChanged(void) {
    computeMap();
    w3d->setMap(sfMap);
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::selectedAll(void) {
    if(popupSender != nullptr) {
        popupSender->selectAll();
    }
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::selectedNone(void) {
    if(popupSender != nullptr) {
        popupSender->clearSelection();
    }
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::popupMenu(const QPoint &pos) {
    popupSender = dynamic_cast<QListWidget *>(sender());

    if(popupSender != nullptr) {
        lvMenu->exec(popupSender->mapToGlobal(pos));
    }
}
//-----------------------------------------------------------------------------------------------
QColor C3dPreview::getColor(QString coul) {
    QColor result = Qt::black;

    if(coul == "15") {
        result = Qt::white;
    } else if(coul == "4") {
        result = Qt::red;
    } else if(coul == "1") {
        result = Qt::blue;
    } else if(coul == "78") {
        result = QColor(246, 212, 179);
    } else if(coul == "70") {
        result = QColor(88, 42, 18);
    } else if(coul == "14") {
        result = Qt::yellow;
    } else if(coul == "2") {
        result = QColor(0, 140, 14);
    }

    return result;
}
//-----------------------------------------------------------------------------------------------
QString C3dPreview::getColorNameFromIdx(QString coul) {
    QString result = "Noir";

    if(coul == "15") {
        result = "Blanc";
    } else if(coul == "4") {
        result = "Rouge";
    } else if(coul == "1") {
        result = "Bleu";
    } else if(coul == "78") {
        result = "Peau";
    } else if(coul == "70") {
        result = "Marron";
    } else if(coul == "14") {
        result = "Jaune";
    } else if(coul == "2") {
        result = "Vert";
    }

    return result;
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_hsLX_valueChanged(int) {
    float xValue = static_cast<float>(hsLX->value()) / 10.0f;
    float yValue = static_cast<float>(hsLY->value()) / 10.0f;

    w3d->setTranslate(xValue, yValue);
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::on_hsLY_valueChanged(int) {
    float xValue = static_cast<float>(hsLX->value()) / 10.0f;
    float yValue = static_cast<float>(hsLY->value()) / 10.0f;

    w3d->setTranslate(xValue, yValue);
}
//-----------------------------------------------------------------------------------------------
