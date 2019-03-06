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
typedef struct _SFace {
    GLfloat coords[4][3];
    QColor color;
}SFace;
//-----------------------------------------------------------------------------------------------
class C3dView : public QGLWidget {
    Q_OBJECT
public:
    explicit C3dView(QWidget *parent = nullptr);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void setMap(QList<SFace *> map);
    void setRotate(float rotX, float rotY, float rotZ);
    void setTranslate(float tX, float tY);
protected:
    virtual void wheelEvent(QWheelEvent * event);
private:
    float rotX, rotY, rotZ;
    float tX, tY;
    QPoint lastPos;
    float scale;
    QList<SFace *> map;
};
//-----------------------------------------------------------------------------------------------
#endif // C3DVIEW_H
//-----------------------------------------------------------------------------------------------
