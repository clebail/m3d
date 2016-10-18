#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QtDebug>
#include <QHash>
#include <QList>
#include <cstdlib>
#include <common.h>

void loadDatas(QHash<int, QList<SPoint*> *> *map, int count);
void dessiner(QHash<int, QList<SPoint*> *> *map);
void setColor(char codeCoul);

double angleX, angleY, angleZ;
int x, y, z;

int main(int argc, char *argv[]) {
    SDL_Event event;
	QHash<int, QList<SPoint*> *> *map = new QHash<int, QList<SPoint*> *>();
	bool fini = false;
	int count = NB_PAS;
	
	if(argc > 1) {
		count = QString(argv[1]).toInt();
	}

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

	loadDatas(map, count);
    dessiner(map);

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
        dessiner(map);
    }
    
    QHashIterator<int, QList<SPoint*> *> i(*map);
	while (i.hasNext()) {
		QList<SPoint *> *list;
		
		i.next();
		
		list = i.value();
		for(int j=0;j<list->size();j++) {
			delete list->at(j);
		}
		
		delete list;
	}
    
    delete map;

    return 0;
}

void loadDatas(QHash<int, QList<SPoint*> *> *map, int count) {
	for(int i=0;i<count;i++) {
		QString fileName = "../txts/"+QString::number(i+1)+".txt";
		QFile file(fileName);
		
		if(file.open(QIODevice::ReadOnly)) {
			QTextStream txtStream(&file);
			int key = (NB_PAS/2-i)*STEPZ;
			QList<SPoint *> * list = new QList<SPoint *>();
			
			map->insert(key, list);
			
			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");
				SPoint *point = new SPoint;
				
				point->x = fields[0].toInt();
				point->y = fields[1].toInt();
				point->coul = fields[2][0].toAscii();
				
				list->append(point);
			}
			
			file.close();
		}
	}
}

void dessiner(QHash<int, QList<SPoint*> *> *map) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(x, y, z, x, 0, z, 0 ,0, 1);
	
	glRotated(angleZ, 0, 0, 1);
	glRotated(angleY, 0, 1, 0);
    glRotated(angleX, 1, 0, 0);

    glBegin(GL_QUADS);

	QHashIterator<int, QList<SPoint*> *> i(*map);
	while (i.hasNext()) {
		int key, idx, nextKey, j;
		
		i.next();
		
		key = i.key();
		idx = NB_PAS/2 - key / STEPZ;
		
		nextKey = (NB_PAS/2-(idx+1))*STEPZ;
		
		if(map->contains(nextKey)) {
			QList<SPoint *> * list = i.value();
			QList<SPoint *> * nextList = map->value(nextKey);
			SPoint *point;
			
			for(j=0;j<list->size()-1;j++) {
				point = list->at(j);
						
				setColor(point->coul);
				glVertex3i(point->y, point->x, key);
				point = list->at(j+1);
				glVertex3i(point->y, point->x, key);
				point = nextList->at(j);
				glVertex3i(point->y, point->x, nextKey);
				point = nextList->at(j+1);
				glVertex3i(point->y, point->x, nextKey);
			}
			
			j = list->size()-1;
			point = list->at(j);
						
			setColor(point->coul);
			glVertex3i(point->y, point->x, key);
			point = list->at(0);
			glVertex3i(point->y, point->x, key);
			point = nextList->at(j);
			glVertex3i(point->y, point->x, nextKey);
			point = nextList->at(0);
			glVertex3i(point->y, point->x, nextKey);
		}
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
