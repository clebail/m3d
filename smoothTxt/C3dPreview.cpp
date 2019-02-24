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

    this->map = map;

    if(map != nullptr) {
        createList();
        computeMap();
        w3d->setMap(sfMap);
    }
}
//-----------------------------------------------------------------------------------------------
C3dPreview::~C3dPreview(void) {
    clearMap();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::createList(void) {
    QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);

    lvLayers->clear();
    while (it.hasNext()) {
        it.next();

        lvLayers->addItem(it.key());
    }

    lvLayers->selectAll();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::computeMap(void) {
    QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);
    QList<QListWidgetItem *> selectedItems = lvLayers->selectedItems();
    QStringList selectedText;

    clearMap();

    for(int i=0;i<selectedItems.size();i++) {
        selectedText << selectedItems.at(i)->text();
    }

    while (it.hasNext()) {
        QList<QList<SPoint *>*> *list;

        it.next();
        list = it.value();

        if(selectedText.contains(it.key())) {
            for(int i=0;i<list->size();i++) {
                QList<SPoint *> *sl = list->at(i);

                for(int j=0;j<sl->size();j++) {
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
    }

    return result;
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
void C3dPreview::selectedAll(void) {
    lvLayers->selectAll();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::selectedNone(void) {
    lvLayers->clearSelection();
}
//-----------------------------------------------------------------------------------------------
void C3dPreview::popupMenu(const QPoint &pos) {
    lvMenu->exec(lvLayers->mapToGlobal(pos));
}
//-----------------------------------------------------------------------------------------------
