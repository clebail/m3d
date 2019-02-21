//-----------------------------------------------------------------------------------------------
#include <QtDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include "C3dView.h"
//-----------------------------------------------------------------------------------------------
#define COEF        (10.0f)
#define UNIT        (1/COEF)
//-----------------------------------------------------------------------------------------------
C3dView::C3dView(QWidget *parent) : QGLWidget(parent) {
    timer = new QTimer(this);
    rotX = rotY = rotZ = 0.0;
    scale = 1.0f;
    map = nullptr;

    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    timer->start(16);
}
//-----------------------------------------------------------------------------------------------
C3dView::~C3dView(void) {
    delete timer;
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
    if(map != nullptr) {
        QHashIterator<QString, QList<QList<SPoint *>*>*> it(*map);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(rotX / 16.0f, 1.0, 0.0, 0.0);
        glRotatef(rotY / 16.0f, 0.0, 1.0, 0.0);
        glRotatef(rotZ / 16.0f, 0.0, 0.0, 1.0);
        glScalef(scale, scale, scale);

        while (it.hasNext()) {
            QList<QList<SPoint *>*> *list;
			float y;

            it.next();
            list = it.value();
			y = (map->size() / 2 - it.key().toInt()-1) * UNIT;

            for(int k=0;k<list->size();k++) {
                QList<SPoint *> *sl = list->at(k);

                for(int l=0;l<sl->size();l++) {
                    float x = (sl->at(l)->x / STEPX) / COEF;
                    float z = (sl->at(l)->y / STEPY) / COEF;

                    GLfloat coords[6][4][3] = {
                        { { x+UNIT, y-UNIT, z+UNIT }, { x+UNIT, y-UNIT, z-UNIT }, { x+UNIT, y+UNIT, z-UNIT }, { x+UNIT, y+UNIT, z+UNIT } },
                        { { x-UNIT, y-UNIT, z-UNIT }, { x-UNIT, y-UNIT, z+UNIT }, { x-UNIT, y+UNIT, z+UNIT }, { x-UNIT, y+UNIT, z-UNIT } },
                        { { x+UNIT, y-UNIT, z-UNIT }, { x-UNIT, y-UNIT, z-UNIT }, { x-UNIT, y+UNIT, z-UNIT }, { x+UNIT, y+UNIT, z-UNIT } },
                        { { x-UNIT, y-UNIT, z+UNIT }, { x+UNIT, y-UNIT, z+UNIT }, { x+UNIT, y+UNIT, z+UNIT }, { x-UNIT, y+UNIT, z+UNIT } },
                        { { x-UNIT, y-UNIT, z-UNIT }, { x+UNIT, y-UNIT, z-UNIT }, { x+UNIT, y-UNIT, z+UNIT }, { x-UNIT, y-UNIT, z+UNIT } },
                        { { x-UNIT, y+UNIT, z+UNIT }, { x+UNIT, y+UNIT, z+UNIT }, { x+UNIT, y+UNIT, z-UNIT }, { x-UNIT, y+UNIT, z-UNIT } }
                    };

                    for(unsigned int i=0;i<6;++i) {
                        glLoadName(i);
                        glBegin(GL_QUADS);
                        qglColor(getColor(sl->at(l)->coul));

                        for(int j=0;j<4;++j) {
                            glVertex3f(coords[i][j][0], coords[i][j][1], coords[i][j][2]);
                        }
                        glEnd();
                    }
                }
            }

            y -= UNIT;
        }
    }
}
//-----------------------------------------------------------------------------------------------
void C3dView::setMap(QHash<QString, QList<QList<SPoint *>*>*> *map) {
    this->map = map;
}
//-----------------------------------------------------------------------------------------------
void C3dView::mouseMoveEvent(QMouseEvent *event) {
    if(!lastPos.isNull()) {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            rotX = (rotX + 8 * dy);
            rotY = (rotY + 8 * dx);
        }
    }
    lastPos = event->pos();
}
//-----------------------------------------------------------------------------------------------
void C3dView::wheelEvent(QWheelEvent * event) {
    event->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
}
//-----------------------------------------------------------------------------------------------
void C3dView::timeout(void) {
    updateGL();
}
//-----------------------------------------------------------------------------------------------
QColor C3dView::getColor(QString coul) {
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
    }

    return result;
}
//-----------------------------------------------------------------------------------------------
