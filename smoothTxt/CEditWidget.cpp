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
    selectedPoint = selectedList = -1;
    mousePressed = false;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setMap(QList<QList<SPoint *>*> *map) {
    this->map = map;

    selectedPoint = selectedList = -1;
    mousePressed = false;

    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::dropCurrentPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoint != -1) {
        map->at(selectedList)->removeAt(selectedPoint);

        if(map->at(selectedList)->size() == 0) {
            map->removeAt(selectedList);
        }

        selectedPoint = selectedList = -1;
        mousePressed = false;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoint != -1) {
        SPoint *p = new SPoint;

        p->x = 0;
        p->y = 0;
        p->coul = '0';

        map->at(selectedList)->insert(selectedPoint, p);

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::upPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoint != -1) {
        int ou = selectedPoint+1;
        SPoint *p;

        if(ou == map->at(selectedList)->size()) {
            ou = 0;
        }
        p = map->at(selectedList)->takeAt(selectedPoint);

        map->at(selectedList)->insert(ou, p);
        selectedPoint = ou;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::downPoint(void) {
    if(map != 0 && selectedList != -1 && selectedPoint != -1) {
        int ou = selectedPoint-1;
        SPoint *p;

        if(ou == -1) {
            ou = map->at(selectedList)->size()-1;
        }
        p = map->at(selectedList)->takeAt(selectedPoint);

        map->at(selectedList)->insert(ou, p);
        selectedPoint = ou;

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
    if(map != 0 && selectedList != -1 && selectedPoint != -1) {
         map->at(selectedList)->at(selectedPoint)->coul = color;

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

                if(i == selectedList && j == selectedPoint) {
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

        if(mousePressed && selectedList != -1 && selectedPoint != -1) {
            SPoint *p = map->at(selectedList)->at(selectedPoint);

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

    if(map != 0) {
        int i,j;
        int x = event->x() - zeroX;
        int y = event->y() - zeroY;


        x += x < 0 ? -STEPX/2 : STEPX/2;
        y += y < 0 ? -STEPX/2 : STEPX/2;

        x = (x/STEPX)*STEPX;
        y = (y/STEPX)*STEPX;

        selectedList = -1;
        selectedPoint = -1;

        for(i=0;i<map->size();i++) {
            QList<SPoint *> *list = map->at(i);

            for(j=0;j<list->size();j++) {
                SPoint *p = list->at(j);

                if(p->x == x && p->y == y) {
                    selectedList = i;
                    selectedPoint = j;

                    break;
                }
            }
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
