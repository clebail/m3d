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
typedef struct _SCube {
    GLfloat coords[6][4][3];
    QColor color;
}SCube;
//-----------------------------------------------------------------------------------------------
class C3dView : public QGLWidget {
    Q_OBJECT
public:
    explicit C3dView(QWidget *parent = nullptr);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void setMap(QList<SCube *> map);
    void setRotate(float rotX, float rotY, float rotZ);
protected:
    virtual void wheelEvent(QWheelEvent * event);
private:
    float rotX, rotY, rotZ;
    QPoint lastPos;
    float scale;
    QList<SCube *> map;
};
//-----------------------------------------------------------------------------------------------
#endif // C3DVIEW_H
//-----------------------------------------------------------------------------------------------
