//-----------------------------------------------------------------------------------------------
#include <QPaintEvent>
#include <QPainter>
#include <QtDebug>
#include <QMouseEvent>
#include <QResizeEvent>
#include <common.h>
#include "CEditWidget.h"
//-----------------------------------------------------------------------------------------------
CEditWidget::CEditWidget(QWidget *parent) : QWidget(parent) {
    map = 0;
    setMouseTracking(true);
    mouseX = mouseY = 0;
    selectedList = -1;
    selectedPoints.clear();
    mousePressed = false;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setMap(QList<QList<SPoint *>*> *map) {
    this->map = map;

    selectedList = -1;
    selectedPoints.clear();
    mousePressed = false;

    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::dropCurrentPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoints.size() != 0) {
        for(int i=0;i<selectedPoints.size();i++) {
            map->at(selectedList)->removeAt(selectedPoints.at(i));
        }

        if(map->at(selectedList)->size() == 0) {
            map->removeAt(selectedList);
        }

        selectedList = -1;
        selectedPoints.clear();
        mousePressed = false;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoints.size() != 0) {
        SPoint *p = new SPoint;

        p->x = 0;
        p->y = 0;
        p->coul = '0';

        map->at(selectedList)->insert(selectedPoints.at(0), p);

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::upPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoints.size() == 1) {
        int ou = selectedPoints.at(0)+1;
        SPoint *p;

        if(ou == map->at(selectedList)->size()) {
            ou = 0;
        }
        p = map->at(selectedList)->takeAt(selectedPoints.at(0));

        map->at(selectedList)->insert(ou, p);
        selectedPoints[0] = ou;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::downPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoints.size() == 1) {
        int ou = selectedPoints.at(0)-1;
        SPoint *p;

        if(ou == -1) {
            ou = map->at(selectedList)->size()-1;
        }
        p = map->at(selectedList)->takeAt(selectedPoints.at(0));

        map->at(selectedList)->insert(ou, p);
        selectedPoints[0] = ou;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addGroupe(void) {
    if(map != 0) {
        QList<SPoint *> *list =new QList<SPoint *>();
        SPoint *p = new SPoint;

        p->x = 0;
        p->y = 0;
        p->coul = '0';

        list->append(p);

        map->append(list);

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setColor(QString color) {
    if(map != 0 && selectedList != -1 && selectedPoints.size() != 0) {
        for(int i=0;i<selectedPoints.size();i++) {
            map->at(selectedList)->at(selectedPoints.at(i))->coul = color;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplir(void) {
    if(map != 0 && selectedList != -1) {
         remplirLine();
         remplirSurface();

         repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    QRect rect = event->rect().adjusted(0, 0, -1, -1);
    QPen linePen(Qt::black);
    QPen selectedPen(Qt::black);
    int x, y;

    linePen.setStyle(Qt::DotLine);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);

    painter.drawRect(rect);

    painter.setPen(linePen);

    for(y=zeroY;y>0;y-=STEPY) {
        painter.drawLine(1, y, rect.width()-1, y);
    }

    for(y=zeroY+STEPY;y<rect.height();y+=STEPY) {
        painter.drawLine(1, y, rect.width()-1, y);
    }

    for(x=zeroX;x>0;x-=STEPX) {
        painter.drawLine(x, 1, x, rect.height()-1);
    }

    for(x=zeroX+STEPX;x<rect.width();x+=STEPX) {
        painter.drawLine(x, 1, x, rect.height()-1);
    }

    if(map != 0) {
        int i,j;



        for(i=0;i<map->size();i++) {
            QList<SPoint *> *list = map->at(i);
            SPoint *previous = list->at(list->size()-1);

            for(j=0;j<list->size();j++) {
                SPoint *p = list->at(j);
                QColor color = getColor(p->coul);

                painter.setPen(Qt::black);
                painter.setBrush(color);

                painter.drawEllipse(p->x+zeroX-STEPX/4, p->y+zeroY-STEPY/4, STEPX/2, STEPY/2);

                painter.drawLine(previous->x+zeroX, previous->y+zeroY, p->x+zeroX, p->y+zeroY);

                if(i == selectedList && selectedPoints.contains(j)) {
                    painter.setPen(selectedPen);
                    painter.setBrush(Qt::NoBrush);

                    painter.drawEllipse(p->x+zeroX-STEPX/2, p->y+zeroY-STEPY/2, STEPX, STEPY);
                }

                previous = p;
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::mouseMoveEvent(QMouseEvent * event) {
    int x = ((event->x() - zeroX) / STEPX)*STEPX;
    int y = ((event->y() - zeroY) / STEPY)*STEPY;

    if(x != mouseX || y != mouseY) {
        mouseX = x;
        mouseY = y;

        if(mousePressed && selectedList != -1 && selectedPoints.size() == 1) {
            SPoint *p = map->at(selectedList)->at(selectedPoints.at(0));

            p->x = mouseX;
            p->y = mouseY;

            repaint();
        }

        emit(mouseMove(x, y));
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::resizeEvent(QResizeEvent * event) {
    zeroX = event->size().width()/2;
    zeroY = event->size().height()/2;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::mousePressEvent(QMouseEvent * event) {

    if(map != 0 && event->button() == Qt::LeftButton) {
        int i,j;
        int x = event->x() - zeroX;
        int y = event->y() - zeroY;
        bool pointSelected = false;


        x += x < 0 ? -STEPX/2 : STEPX/2;
        y += y < 0 ? -STEPX/2 : STEPX/2;

        x = (x/STEPX)*STEPX;
        y = (y/STEPX)*STEPX;

        for(i=0;i<map->size();i++) {
            QList<SPoint *> *list = map->at(i);

            for(j=0;j<list->size();j++) {
                SPoint *p = list->at(j);

                if(p->x == x && p->y == y) {
                    if(event->modifiers() == Qt::ControlModifier) {
                        if(selectedList == i) {
                            selectedPoints.append(j);
                        }else {
                            selectedList = i;
                            selectedPoints.clear();
                            selectedPoints.append(j);
                        }
                    }else {
                        selectedList = i;
                        selectedPoints.clear();
                        selectedPoints.append(j);
                    }

                    pointSelected = true;

                    break;
                }
            }
        }

        if(!pointSelected) {
            selectedList = -1;
            selectedPoints.clear();
        }

        repaint();
    }

    mousePressed = true;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
}
//-----------------------------------------------------------------------------------------------
QColor CEditWidget::getColor(QString coul) {
    if(coul == "15") {
        return Qt::white;
    }

    if(coul == "4") {
        return Qt::red;
    }

    return Qt::black;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplirLine(void) {
    int i;
    QList<SPoint *> *list = map->at(selectedList);

    i=0;
    while(i<list->size()-1) {
        remplirPoint(list, i, i+1);
        i++;
    }
    remplirPoint(list, list->size()-1, 0);
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplirPoint(QList<SPoint *> *list, int idxP1, int idxP2) {
    SPoint *p1 = list->at(idxP1);
    SPoint *p2 = list->at(idxP2);
    int diffX = p1->x - p2->x, diffY = p1->y - p2->y;

    if(abs(diffX) > STEPX || abs(diffY) > STEPY) {
        SPoint *p = new SPoint;

        p->x = p1->x + (diffX > 0 ? -STEPX : diffX < 0 ? STEPX : 0);
        p->y = p1->y + (diffY > 0 ? -STEPY : diffY < 0 ? STEPY : 0);

        p->coul = p1->coul;

        list->insert(idxP2, p);
        remplirPoint(list, idxP2, idxP2+1);
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplirSurface(void) {
    QList<SPoint *> *list = map->at(selectedList);
    SPoint *p = list->last();
    SPoint *newP = new SPoint;

    newP->coul = p->coul;

    newP->x = p->x+STEPX;
    newP->y = p->y;
    if(!inList(newP) && inSurface(newP)) {
        list->append(newP);
        remplir(newP);

        return;
    }

    newP->x = p->x-STEPX;
    newP->y = p->y;
    if(!inList(newP) && inSurface(newP)) {
        list->append(newP);
        remplir(newP);

        return;
    }

    newP->x = p->x;
    newP->y = p->y+STEPY;
    if(!inList(newP) && inSurface(newP)) {
        list->append(newP);
        remplir(newP);

        return;
    }

    newP->x = p->x;
    newP->y = p->y-STEPY;
    if(!inList(newP) && inSurface(newP)) {
        list->append(newP);
        remplir(newP);

        return;
    }

    delete newP;
}
//-----------------------------------------------------------------------------------------------
bool CEditWidget::inList(SPoint *p) {
    QList<SPoint *> *list = map->at(selectedList);
    int i;

    for(i=0;i<list->size();i++) {
        SPoint *pv = list->at(i);

        if(pv->x == p->x && pv->y == p->y) {
            return true;
        }
    }

    return false;
}
//-----------------------------------------------------------------------------------------------
bool CEditWidget::inSurface(SPoint *p) {
    QList<SPoint *> *list = map->at(selectedList);
    int cn=0, i;

    for(i=0;i<list->size()-1;i++) {
        SPoint *pv = list->at(i);
        SPoint *pv1 = list->at(i+1);

        cn+=testPoint(p, pv, pv1);
    }

    cn+=testPoint(p, list->last(), list->first());

    return cn % 2 != 0;
}
//-----------------------------------------------------------------------------------------------
int CEditWidget::testPoint(SPoint *p, SPoint *pv, SPoint *pv1) {
    if(((pv->y <= p->y) && (pv1->y > p->y)) || ((pv->y > p->y) && (pv1->y <= p->y))) {
        float vt = (float)(p->y - pv->y) / (pv1->y - pv->y);
        if(p->x < pv->x + vt * (pv1->x - pv->x)) {
            return 1;
        }
    }

    return 0;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplir(SPoint *p) {
    QList<SPoint *> *list = map->at(selectedList);
    SPoint *newP = 0;

    newP = new SPoint;
    newP->coul = p->coul;
    newP->x = p->x+STEPX;
    newP->y = p->y;
    if(!inList(newP)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }

    newP->coul = p->coul;
    newP->x = p->x-STEPX;
    newP->y = p->y;
    if(!inList(newP)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }


    newP->coul = p->coul;
    newP->x = p->x;
    newP->y = p->y+STEPY;
    if(!inList(newP)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }

    newP->coul = p->coul;
    newP->x = p->x;
    newP->y = p->y-STEPY;
    if(!inList(newP)) {
        list->append(newP);

        remplir(newP);

        return;
    }

    delete newP;
}
//-----------------------------------------------------------------------------------------------
