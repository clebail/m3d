//-----------------------------------------------------------------------------------------------
#ifndef __CEDITWIDGET_H__
#define __CEDITWIDGET_H__
//-----------------------------------------------------------------------------------------------
#include <QWidget>
#include <QList>
#include <common.h>
//-----------------------------------------------------------------------------------------------
class CEditWidget : public QWidget {
    Q_OBJECT
    public:
        CEditWidget(QWidget *parent = 0);

        void setMaps(QList<QList<SPoint *>*> *map, QList<QList<SPoint *>*> *mDessous, QList<QList<SPoint *>*> *mDessus);
        void dropCurrentPoint(void);
        void addPoint(void);
        void addGroupe(void);
        void setColor(QString color);
        void remplir(void);
        void zapLigne(void);
        void addLigne(void);
        void plusX(void);
        void moinsX(void);
        void plusY(void);
        void moinsY(void);
        void simplify(void);
        void setShowDessous(bool show);
        void setShowDessus(bool show);
    protected:
        virtual void paintEvent(QPaintEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void resizeEvent(QResizeEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
    private:
        QList<QList<SPoint *>*> *map;
        QList<QList<SPoint *>*> *mapDessous;
        QList<QList<SPoint *>*> *mapDessus;
        int zeroX, zeroY;
        int mouseX, mouseY;
        int selectedList;
        bool mousePressed;
        QList<int> selectedPoints;
        bool showDessous, showDessus;

        QColor getColor(QString coul, bool real);
        void remplirLine(void);
        void remplirPoint(QList<SPoint *> *list, int idxP1, int idxP2);
        void remplirSurface(void);
        bool inList(SPoint *p, int *pos);
        bool inSurface(SPoint *p);
        int testPoint(SPoint *p, SPoint *pv, SPoint *pv1);
        void remplir(SPoint *p);
        bool isContour(SPoint *p);
        void draw(QList<QList<SPoint *>*> *map, bool real, QPainter *painter);
    signals:
        void mouseMove(int x, int y);
};
//-----------------------------------------------------------------------------------------------
#endif // __CEDITWIDGET_H__
//-----------------------------------------------------------------------------------------------
