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

        void setMap(QList<QList<SPoint *>*> *map);
        void dropCurrentPoint(void);
        void addPoint(void);
        void upPoint(void);
        void downPoint(void);
        void addGroupe(void);
        void setColor(QString color);
        void remplir(void);
        void zapLigne(void);
        void addLigne(void);
    protected:
        virtual void paintEvent(QPaintEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void resizeEvent(QResizeEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
    private:
        QList<QList<SPoint *>*> *map;
        int zeroX, zeroY;
        int mouseX, mouseY;
        int selectedList;
        bool mousePressed;
        QList<int> selectedPoints;

        QColor getColor(QString coul);
        void remplirLine(void);
        void remplirPoint(QList<SPoint *> *list, int idxP1, int idxP2);
        void remplirSurface(void);
        bool inList(SPoint *p);
        bool inSurface(SPoint *p);
        int testPoint(SPoint *p, SPoint *pv, SPoint *pv1);
        void remplir(SPoint *p);
    signals:
        void mouseMove(int x, int y);
};
//-----------------------------------------------------------------------------------------------
#endif // __CEDITWIDGET_H__
//-----------------------------------------------------------------------------------------------
