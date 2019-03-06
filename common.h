#ifndef __COMMON_H__
#define __COMMON_H__

#include <QString>
#include <QColor>
#include <QtDebug>
#include <math.h>

#define NB_IMAGE			72
#define IMAGE_WIDTH			1200
#define IMAGE_HEIGHT		1200
#define NB_PAS				80
#define MIDDLE				(IMAGE_WIDTH/2)
#define STEPY				(IMAGE_HEIGHT/NB_PAS)
#define STEPX				(IMAGE_WIDTH/NB_PAS)
#define STEPZ				15
#define PI					(3.14159)
#define SURFACE_CASE		(STEPX*STEPY/2)
#define OFFSET_ANGLE		2
#define ROND_COEF			(0.3075)
#define TXTS_FOLDER         (QString("../txts/"))
#define SVGS_FOLDER         (QString("../svgs/"))
#define LDRS_FOLDER         (QString("../ldrs/"))
#define BINVOX_FOLDER       (QString("../binvox/"))

typedef struct _SPoint {
	int x,y;
    QString coul;

    _SPoint(void) {
        this->x = 0;
        this->y = 0;
    }

    _SPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(struct _SPoint const& other) {
        return x == other.x && y == other.y;
    }
}SPoint;

#endif //__COMMON_H__
