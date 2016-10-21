//-----------------------------------------------------------------------------------------------
#ifndef __CCOLORLABEL_H__
#define __CCOLORLABEL_H__
//-----------------------------------------------------------------------------------------------
#include <QLabel>
//-----------------------------------------------------------------------------------------------
class CColorLabel : public QLabel {
    Q_OBJECT
    public:
        CColorLabel(QWidget *parent = 0);
    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent * event);
    signals:
        void doubleClicked(void);
};
//-----------------------------------------------------------------------------------------------
#endif // __CCOLORLABEL_H__
//-----------------------------------------------------------------------------------------------
