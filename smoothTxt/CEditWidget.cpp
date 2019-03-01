//-----------------------------------------------------------------------------------------------
#include <QPaintEvent>
#include <QPainter>
#include <QtDebug>
#include <QMouseEvent>
#include <QResizeEvent>
#include <common.h>
#include "CEditWidget.h"
#include "CContourLess.h"
//-----------------------------------------------------------------------------------------------
struct SInsideLess {
    bool operator()(struct _SPoint *a, struct _SPoint *b) const {
        if(a->y == b->y) {
            return a->x < b->x;
        }

        return a->y < b->y;
    }
};
//-----------------------------------------------------------------------------------------------
CEditWidget::CEditWidget(QWidget *parent) : QWidget(parent) {
    map = mapDessus = nullptr;
    setMouseTracking(true);
    mouseX = mouseY = 0;
    selectedList = -1;
    selectedPoints.clear();
    mousePressed = false;
    showDessus = false;
    showInverse = false;
    stepx = STEPX;
    stepy = STEPY;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setMaps(QList<QList<SPoint *>*> *map, QList<QList<SPoint *>*> *mDessus) {
    this->map = map;
    this->mapDessus = mDessus;

    selectedList = -1;
    selectedPoints.clear();
    mousePressed = false;

    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::dropCurrentPoint(void) {
    if(map != nullptr && selectedList != -1 && selectedPoints.size() != 0) {
        int last;
        int i;

        qSort(selectedPoints.begin(), selectedPoints.end());
        for(i=selectedPoints.size()-1;i>=0;i--) {
            map->at(selectedList)->removeAt(selectedPoints.at(i));
        }

        if(map->at(selectedList)->size() == 0) {
            map->removeAt(selectedList);

            selectedList = -1;
            selectedPoints.clear();
        }else {
            last = selectedPoints.at(0)-1;
            if(last == -1) {
                last = map->at(selectedList)->size()-1;
            }

            selectedPoints.clear();
            selectedPoints.append(last);
        }

        mousePressed = false;

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addPoint(void) {
    if(map != nullptr && selectedList != -1) {
        SPoint *nP = new SPoint;
        QPoint p = mapFromGlobal(QCursor::pos());

        int x = p.x() - zeroX;
        int y = p.y() - zeroY;

        x += x < 0 ? -stepx/2 : stepx/2;
        y += y < 0 ? -stepy/2 : stepy/2;

        x = (x/stepx)*stepx;
        y = (y/stepy)*stepy;

        x = (x/stepx)*STEPX;
        y = (y/stepy)*STEPY;

        nP->x = x;
        nP->y = y;

        if(selectedPoints.size() != 0) {
            nP->coul = map->at(selectedList)->at(selectedPoints.at(0))->coul;
        } else {
            nP->coul = "0";
        }

        map->at(selectedList)->insert(selectedPoints.at(0), nP);

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addGroupe(void) {
    if(map != nullptr) {
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
    if(map != nullptr && selectedList != -1 && selectedPoints.size() != 0) {
        for(int i=0;i<selectedPoints.size();i++) {
            map->at(selectedList)->at(selectedPoints.at(i))->coul = color;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplir(void) {
	if(map != 0 && selectedList != -1) {
		QPoint p = mapFromGlobal(QCursor::pos());

		int x = p.x() - zeroX;
		int y = p.y() - zeroY;

        x += x < 0 ? -stepx/2 : stepx/2;
        y += y < 0 ? -stepy/2 : stepy/2;

        x = (x/stepx)*stepx;
        y = (y/stepy)*stepy;

        x = (x/stepx)*STEPX;
        y = (y/stepy)*STEPY;
		
		remplitPoint(x, y);

		repaint();
	}
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::zapLigne(void) {
    if(map != nullptr && selectedList != -1 && selectedPoints.size() == 1) {
        int i = selectedPoints.at(0);
        QList<SPoint *> *list = map->at(selectedList);
        int y = list->at(i)->y;
        int j =0;

        while(j<list->size()) {
            if(list->at(j)->y == y) {
                delete list->at(j);
                list->removeAt(j);
                continue;
            }

            j++;
        }

        for(j=0;j<list->size();j++) {
            if(list->at(j)->y > y) {
                list->at(j)->y-=stepy;
            }
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::addLigne(void) {
    if(map != nullptr && selectedList != -1 && selectedPoints.size() == 1) {
        int i = selectedPoints.at(0);
        QList<SPoint *> *list = map->at(selectedList);
        int y = list->at(i)->y;
        int j =0;

        for(j=0;j<list->size();j++) {
            if(list->at(j)->y > y) {
                list->at(j)->y+=stepy;
            }
        }

        for(j=0;j<list->size();j++) {
            if(list->at(j)->y == y) {
                SPoint *p = new SPoint;

                p->x = list->at(j)->x;
                p->y = y+stepy;
                p->coul = list->at(j)->coul;

                list->append(p);
            }
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::plusX(void) {
    if(map != nullptr && selectedList != -1) {
        QList<SPoint *> *list = map->at(selectedList);
        int i;

        for(i=0;i<list->size();i++) {
            list->at(i)->x+=stepx;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::moinsX(void) {
    if(map != nullptr && selectedList != -1) {
        QList<SPoint *> *list = map->at(selectedList);
        int i;

        for(i=0;i<list->size();i++) {
            list->at(i)->x-=stepx;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::plusY(void) {
    if(map != nullptr && selectedList != -1) {
        QList<SPoint *> *list = map->at(selectedList);
        int i;

        for(i=0;i<list->size();i++) {
            list->at(i)->y+=stepy;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::moinsY(void) {
    if(map != nullptr && selectedList != -1) {
        QList<SPoint *> *list = map->at(selectedList);
        int i;

        for(i=0;i<list->size();i++) {
            list->at(i)->y-=stepy;
        }

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::simplify(void) {
    if(map != nullptr && selectedList != -1) {
        QList<SPoint *> *list = map->at(selectedList);
        QList<SPoint *> contour;
        QList<SPoint *> others;
        int i;

        qSort(list->begin(), list->end(), SInsideLess());

        i=1;
        while(i != list->size()) {
            SPoint *p1=list->at(i-1), *p2 = list->at(i);

            if(*p1 == *p2) {
                delete p2;
                list->removeAt(i);
            } else {
                i++;
            }
        }

        for(i=0;i<list->size();i++) {
            if(isContour(list->at(i))) {
                contour << list->at(i);
            }else {
                others << list->at(i);
            }
        }

        list->clear();

        qSort(others.begin(), others.end(), SInsideLess());

        if(contour.size() != 0) {
            qSort(contour.begin(), contour.end(), CContourLess());

            list->append(contour);
        }

        list->append(others);

        selectedList = -1;
        selectedPoints.clear();

        repaint();
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setShowInverse(bool inverse) {
    showInverse = inverse;
    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setShowDessus(bool show) {
    showDessus = show;
    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::dedouble(void) {
    if(map != nullptr) {
        for(int i=0;i<map->size();i++) {
            QList<SPoint *> *points = map->at(i);
            int j=0;
            while(j<points->size()-1) {
                SPoint *point = points->at(j);
                int k=j+1;
                while(k<points->size()) {
                    if(*point == *(points->at(k))) {
                        delete points->takeAt(k);
                    }else {
                        k++;
                    }
                }
                j++;
            }
        }
    }
    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::setSteps(int stepx, int stepy) {
    this->stepx = stepx;
    this->stepy = stepy;

    repaint();
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QRect rect = geometry();
    QPen linePen(Qt::black);
    int x, y;

    linePen.setStyle(Qt::DotLine);

    painter.setBrush(Qt::white);
    painter.setPen(linePen);

    for(y=zeroY;y>0;y-=stepy) {
        painter.drawLine(1, y, rect.width()-1, y);
    }

    for(y=zeroY+stepy;y<rect.height();y+=stepy) {
        painter.drawLine(1, y, rect.width()-1, y);
    }

    for(x=zeroX;x>0;x-=stepx) {
        painter.drawLine(x, 1, x, rect.height()-1);
    }

    for(x=zeroX+stepx;x<rect.width();x+=stepx) {
        painter.drawLine(x, 1, x, rect.height()-1);
    }

    if(showDessus) {
        if(showInverse) {
            draw(mapDessus, false, &painter);
            draw(map, true, &painter);
        } else {
            draw(map, true, &painter);
            draw(mapDessus, false, &painter);
        }
    } else {
        draw(map, true, &painter);
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::mouseMoveEvent(QMouseEvent * event) {
    int x = ((event->x() - zeroX) / stepx)*stepx;
    int y = ((event->y() - zeroY) / stepy)*stepy;

    if(x != mouseX || y != mouseY) {
        mouseX = x;
        mouseY = y;

        if(mousePressed && selectedList != -1 && selectedPoints.size() == 1) {
            SPoint *p = map->at(selectedList)->at(selectedPoints.at(0));

            p->x = x;
            p->y = y;

            repaint();
        }

        emit(mouseMove((x/stepx)*STEPX, (y/stepy)*STEPY));
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::resizeEvent(QResizeEvent * event) {
    zeroX = event->size().width()/2;
    zeroY = event->size().height()/2;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::mousePressEvent(QMouseEvent * event) {

    if(map != nullptr && event->button() == Qt::LeftButton) {
        int i,j;
        int x = event->x() - zeroX;
        int y = event->y() - zeroY;
        bool pointSelected = false;


        x += x < 0 ? -stepx/2 : stepx/2;
        y += y < 0 ? -stepy/2 : stepy/2;

        x = (x/stepx)*stepx;
        y = (y/stepy)*stepy;

        x = (x/stepx)*STEPX;
        y = (y/stepy)*STEPY;

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
bool CEditWidget::inList(SPoint *p, int *pos) {
    QList<SPoint *> *list = map->at(selectedList);
    int i;

    for(i=0;i<list->size();i++) {
        SPoint *pv = list->at(i);

        if(*pv == *p) {
            *pos = i;
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
        float vt = static_cast<float>((p->y - pv->y) / (pv1->y - pv->y));
        if(p->x < pv->x + vt * (pv1->x - pv->x)) {
            return 1;
        }
    }

    return 0;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplir(SPoint *p) {
    QList<SPoint *> *list = map->at(selectedList);
    SPoint *newP = nullptr;
    int newPIdx;

    newP = new SPoint;
    newP->coul = p->coul;
    newP->x = p->x+stepx;
    newP->y = p->y;
    if(!inList(newP, &newPIdx)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }

    newP->coul = p->coul;
    newP->x = p->x-stepx;
    newP->y = p->y;
    if(!inList(newP, &newPIdx)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }


    newP->coul = p->coul;
    newP->x = p->x;
    newP->y = p->y+stepy;
    if(!inList(newP, &newPIdx)) {
        list->append(newP);

        remplir(newP);
        newP = new SPoint;
    }

    newP->coul = p->coul;
    newP->x = p->x;
    newP->y = p->y-stepy;
    if(!inList(newP, &newPIdx)) {
        list->append(newP);

        remplir(newP);

        return;
    }

    delete newP;
}
//-----------------------------------------------------------------------------------------------
bool CEditWidget::isContour(SPoint *p) {
    SPoint tP;
    int tPIdx;

    tP.x = p->x;
    tP.y = p->y - stepy;
    if(!inList(&tP, &tPIdx)) {
        return true;
    }

    tP.x = p->x + stepx;
    tP.y = p->y;
    if(!inList(&tP, &tPIdx)) {
        return true;
    }

    tP.x = p->x;
    tP.y = p->y + stepy;
    if(!inList(&tP, &tPIdx)) {
        return true;
    }

    tP.x = p->x - stepx;
    tP.y = p->y;
    if(!inList(&tP, &tPIdx)) {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::draw(QList<QList<SPoint *>*> *map, bool real, QPainter *painter) {
    if(map != nullptr) {
        int i,j;
        QPen selectedPen(Qt::black);

        for(i=0;i<map->size();i++) {
            QList<SPoint *> *list = map->at(i);

            for(j=0;j<list->size();j++) {
                SPoint *p = list->at(j);
                QColor color = getColor(p->coul);
                QBrush brush(real ? color : Qt::black);

                painter->setPen(Qt::black);
                painter->setBrush(brush);


                painter->drawEllipse((p->x/STEPX)*stepx+zeroX-stepx/4, (p->y/STEPY)*stepy+zeroY-stepy/4, stepx/2, stepy/2);

                if(i == selectedList && selectedPoints.contains(j)) {
                    painter->setPen(selectedPen);
                    painter->setBrush(Qt::NoBrush);

                    painter->drawEllipse((p->x/STEPX)*stepx+zeroX-stepx/2, (p->y/STEPY)*stepy+zeroY-stepy/2, stepx, stepy);
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------
void CEditWidget::remplitPoint(int x, int y) {
	QList<SPoint *> *list = map->at(selectedList);
	SPoint *p;
    int i;

    for(i=0;i<list->size();i++) {
        SPoint *pv = list->at(i);

        if(pv->x == x && pv->y == y) {
			return;
		}
    }
    
    p = new SPoint;
	p->x = x;
	p->y = y;
	
	if(selectedPoints.size() != 0) {
		p->coul = map->at(selectedList)->at(selectedPoints.at(0))->coul;
	} else {
		p->coul = "0";
	}
	
	list->append(p);
	
    remplitPoint(x + STEPX, y);
    remplitPoint(x - STEPX, y);
    remplitPoint(x, y + STEPY);
    remplitPoint(x, y - STEPY);
}
//-----------------------------------------------------------------------------------------------
