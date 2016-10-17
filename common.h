#ifndef __COMMON_H__
#define __COMMON_H__
#define NB_IMAGE			72
#define IMAGE_WIDTH			1200
#define IMAGE_HEIGHT		1200
#define NB_PAS				50
#define MIDDLE				(IMAGE_WIDTH/2)
#define STEPY				(IMAGE_HEIGHT/NB_PAS)
#define STEPX				(IMAGE_WIDTH/NB_PAS)
#define PI					((double)3.14159)
#define SURFACE_CASE		(STEPX*STEPY/2)

typedef struct _SPoint {
	int x,y;
	char coul;
}SPoint;
#endif //__COMMON_H__
