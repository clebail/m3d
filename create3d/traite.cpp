#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QtDebug>
#include <QList>
#include <cstdlib>
#include <common.h>

typedef struct _SPoint3d : SPoint {
	int z;
}SPoint3d;

void loadDatas(QList<SPoint3d *> *points);
void dessiner(QList<SPoint3d *> *points);
void setColor(char codeCoul);

double angleX, angleY, angleZ;
int x, y, z;

int main(void) {
    SDL_Event event;
	QList<SPoint3d *> *points = new QList<SPoint3d *>();
	bool fini = false;

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70, (double)640/480, 1, 1000);
    glEnable(GL_DEPTH_TEST);
	
	angleX = angleY = angleZ = 0.0;
	x = z = 0;
	y = 1000;

	loadDatas(points);
    dessiner(points);

    while(!fini) {
        SDL_WaitEvent(&event);

        switch(event.type) {
            case SDL_QUIT:
				fini = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_x:
						angleX+=event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT ? -10 : 10;
						break;
					case SDLK_y:
						angleY+=event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT ? -10 : 10;
						break;
					case SDLK_z:
						angleZ+=event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT ? -10 : 10;
						break;
					case SDLK_ESCAPE:
						fini = true;
						break;
					case SDLK_UP:
						z+=10;
						break;
					case SDLK_DOWN:
						z-=10;
						break;
					case SDLK_RIGHT:
						x+=10;
						break;
					case SDLK_LEFT:
						x-=10;
						break;
					case SDLK_KP_PLUS:
						y-=10;
						break;
					case SDLK_KP_MINUS:
						y+=10;
						break;
					default:
						break;
				}
				
				qDebug() << angleX << angleY << angleZ;
				
				break;
			default:
				break;
						
        }
        dessiner(points);
    }
    
    for (int i=0;i<points->size();i++) {
		delete points->at(i);
    }
    
    delete points;

    return 0;
}

void loadDatas(QList<SPoint3d *> *points) {
	for(int i=0;i<NB_PAS;i++) {
		QString fileName = "../txts/"+QString::number(i+1)+".txt";
		QFile file(fileName);
		
		if(file.open(QIODevice::ReadOnly)) {
			QTextStream txtStream(&file);

			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");
				SPoint3d *point = new SPoint3d;
				
				point->x = fields[0].toInt();
				point->y = fields[1].toInt();
				point->z = (NB_PAS/2-i)*STEPZ;
				point->coul = fields[2][0].toAscii();
				
				points->append(point);
			}
			
			file.close();
		}
	}
}

void dessiner(QList<SPoint3d *> *points) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(x, y, z, x, 0, z, 0 ,0, 1);
	
	glRotated(angleZ, 0, 0, 1);
	glRotated(angleY, 0, 1, 0);
    glRotated(angleX, 1, 0, 0);

    glBegin(GL_POINTS);

	for (int i=0;i<points->size();i++) {
		SPoint3d *point = points->at(i);
		
		setColor(point->coul);
		glVertex3i(point->y, point->x, point->z);
	}

    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}

void setColor(char codeCoul) {
	switch(codeCoul) {
		case '4':
		default:
			glColor3ub(255,0,0);
	}
}
