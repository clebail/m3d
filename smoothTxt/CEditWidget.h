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
        int selectedPoint, selectedList;
        bool mousePressed;
    signals:
        void mouseMove(int x, int y);
};
//-----------------------------------------------------------------------------------------------
#endif // __CEDITWIDGET_H__
//-----------------------------------------------------------------------------------------------
