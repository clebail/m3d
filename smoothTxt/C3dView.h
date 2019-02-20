//-----------------------------------------------------------------------------------------------
#ifndef C3DVIEW_H
#define C3DVIEW_H
//-----------------------------------------------------------------------------------------------
#include <QGLWidget>
#include <QTimer>
#include <QHash>
#include <QList>
#include <QColor>
#include "common.h"
//-----------------------------------------------------------------------------------------------
class C3dView : public QGLWidget {
    Q_OBJECT
public:
    explicit C3dView(QWidget *parent = nullptr);
    ~C3dView(void);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void setMap(QHash<QString, QList<QList<SPoint *>*>*> *map);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent * event);
private:
    QTimer *timer;
    float rotX, rotY, rotZ;
    QPoint lastPos;
    float scale;
    QHash<QString, QList<QList<SPoint *>*>*> *map;
    QColor getColor(QString coul);
private slots:
    void timeout(void);
};
//-----------------------------------------------------------------------------------------------
#endif // C3DVIEW_H
//-----------------------------------------------------------------------------------------------
