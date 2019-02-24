//-----------------------------------------------------------------------------------------------
#include <QtDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include "C3dView.h"
//-----------------------------------------------------------------------------------------------
C3dView::C3dView(QWidget *parent) : QGLWidget(parent) {
    rotX = rotY = rotZ = 0.0;
    scale = 1.0f;
}
//-----------------------------------------------------------------------------------------------
void C3dView::initializeGL() {
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
//-----------------------------------------------------------------------------------------------
void C3dView::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble x = static_cast<GLdouble>(width / height);
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}
//-----------------------------------------------------------------------------------------------
void C3dView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(map.size()) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);

        glRotatef(rotX, 1.0, 0.0, 0.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotZ, 0.0, 0.0, 1.0);

        glScalef(scale, scale, scale);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for(int i=0;i<map.size();i++) {
            //glLoadName(j);
            glBegin(GL_QUADS);
            qglColor(Qt::gray);

            for(int k=0;k<4;++k) {
                glVertex3f(map.at(i)->coords[k][0], map.at(i)->coords[k][1], map.at(i)->coords[k][2]);
            }
            glEnd();
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);

        for(int i=0;i<map.size();i++) {
            //for(unsigned int j=0;j<6;++j) {
                glLoadName(j);
                glBegin(GL_QUADS);
                qglColor(map.at(i)->color);

                for(int k=0;k<4;++k) {
                    glVertex3f(map.at(i)->coords[k][0], map.at(i)->coords[k][1], map.at(i)->coords[k][2]);
                }
                glEnd();
           // }
        }
    }
}
//-----------------------------------------------------------------------------------------------
void C3dView::setMap(QList<SFace *> map) {
    this->map = map;

    updateGL();
}
//-----------------------------------------------------------------------------------------------
void C3dView::setRotate(float rotX, float rotY, float rotZ) {
    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;

    updateGL();
}
//-----------------------------------------------------------------------------------------------
void C3dView::wheelEvent(QWheelEvent * event) {
    event->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
    updateGL();
}
//-----------------------------------------------------------------------------------------------
