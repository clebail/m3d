//-----------------------------------------------------------------------------------------------
#include "C3dPreview.h"
//-----------------------------------------------------------------------------------------------
#define COEF        (10.0f)
#define UNIT        (1/COEF)
#define UNIT2       (UNIT/2)
//-----------------------------------------------------------------------------------------------
C3dPreview::C3dPreview(QWidget *parent, QHash<QString, QList<QList<SPoint *>*>*> *map) : QDialog(parent) {
    setupUi(this);

    lvLayers->setSelectionMode(QAbstractItemView::MultiSelection);

    this->map = map;

    if(map != nullptr) {
        createList();
        w3d->setMap(computeMap());
    }
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
QList<SCube *> C3dPreview::computeMap(void) {
    QList<SCube *> result;
    QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);
    QList<QListWidgetItem *> selectedItems = lvLayers->selectedItems();
    QStringList selectedText;

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

                for(int l=0;l<sl->size();l++) {
                    float x = (sl->at(l)->x / STEPX) / COEF;;
                    float y = (map->size() / 2 - it.key().toInt() - 1) * UNIT;
                    float z = (sl->at(l)->y / STEPY) / COEF;

                    SCube * cube = new SCube();

                     GLfloat coords[6][4][3] = {
                        { { x+UNIT2, y-UNIT2, z+UNIT2 }, { x+UNIT2, y-UNIT2, z-UNIT2 }, { x+UNIT2, y+UNIT2, z-UNIT2 }, { x+UNIT2, y+UNIT2, z+UNIT2 } },
                        { { x-UNIT2, y-UNIT2, z-UNIT2 }, { x-UNIT2, y-UNIT2, z+UNIT2 }, { x-UNIT2, y+UNIT2, z+UNIT2 }, { x-UNIT2, y+UNIT2, z-UNIT2 } },
                        { { x+UNIT2, y-UNIT2, z-UNIT2 }, { x-UNIT2, y-UNIT2, z-UNIT2 }, { x-UNIT2, y+UNIT2, z-UNIT2 }, { x+UNIT2, y+UNIT2, z-UNIT2 } },
                        { { x-UNIT2, y-UNIT2, z+UNIT2 }, { x+UNIT2, y-UNIT2, z+UNIT2 }, { x+UNIT2, y+UNIT2, z+UNIT2 }, { x-UNIT2, y+UNIT2, z+UNIT2 } },
                        { { x-UNIT2, y-UNIT2, z-UNIT2 }, { x+UNIT2, y-UNIT2, z-UNIT2 }, { x+UNIT2, y-UNIT2, z+UNIT2 }, { x-UNIT2, y-UNIT2, z+UNIT2 } },
                        { { x-UNIT2, y+UNIT2, z+UNIT2 }, { x+UNIT2, y+UNIT2, z+UNIT2 }, { x+UNIT2, y+UNIT2, z-UNIT2 }, { x-UNIT2, y+UNIT2, z-UNIT2 } }
                    };

                    memcpy(&cube->coords, &coords, sizeof(GLfloat[6][4][3]));
                    cube->color = getColor(sl->at(l)->coul);

                    result.append(cube);
                }
            }
        }
    }

    return result;
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
    w3d->setMap(computeMap());
}
//-----------------------------------------------------------------------------------------------
