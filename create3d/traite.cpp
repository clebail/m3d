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

#define STEPX2      (STEPX/2)
#define STEPY2      (STEPY/2)

void loadDatas(QHash<int, QList<QList<SPoint*>*>*> *map, int count);
void dessiner(QHash<int, QList<QList<SPoint*>*>*> *map);
void setColor(QString codeCoul);

double angleX, angleY, angleZ;
int x, y, z;

int main(int argc, char *argv[]) {
    SDL_Event event;
    QHash<int, QList<QList<SPoint*>*>*> *map = new QHash<int, QList<QList<SPoint*>*>*>();
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
    
    QHashIterator<int, QList<QList<SPoint*>*>*> i(*map);
	while (i.hasNext()) {
        QList<QList<SPoint*>*> *list;
		
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

void loadDatas(QHash<int, QList<QList<SPoint*>*>*> *map, int count) {
	for(int i=0;i<count;i++) {
        QString fileName = "../txts/"+QString::number(i+1).rightJustified(4, '0')+".txt";
		QFile file(fileName);
        QList<QList<SPoint *>*> *mainList = new QList<QList<SPoint *>*> ();
        QList<SPoint *> *list = 0;
		
		if(file.open(QIODevice::ReadOnly)) {
			QTextStream txtStream(&file);
            int key = i;
			
            map->insert(key, mainList);
			
            int oldId = 0;
			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");
				SPoint *point = new SPoint;
                int id = fields[3].toInt();

                point->x = fields[0].toInt();
                point->y = fields[1].toInt();
                point->coul = fields[2];

                if(id != oldId) {
                    list = new QList<SPoint *>();
                    mainList->append(list);
                }
				
				list->append(point);
                oldId = id;
			}
			
			file.close();
		}
	}
}

void dessiner(QHash<int,  QList<QList<SPoint*>*>*> *map) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(x, y, z, x, 0, z, 0 ,0, 1);
	
	glRotated(angleZ, 0, 0, 1);
	glRotated(angleY, 0, 1, 0);
    glRotated(angleX, 1, 0, 0);


    QHashIterator<int, QList<QList<SPoint*>*>*> i(*map);
	while (i.hasNext()) {
        i.next();
		
        int key = i.key();
        QList<QList<SPoint*>*> *mainList = i.value();

        for(int j=0;j<mainList->size();j++) {
            QList<SPoint*> *list = mainList->at(j);
            for(int k=0;k<list->size();k++) {
                SPoint *point = list->at(k);
                int z1 = (NB_PAS/2-key)*STEPZ;
                int z2 = (NB_PAS/2-key+1)*STEPZ;

                glBegin(GL_QUADS);

				setColor(point->coul);

                glVertex3i(point->x-STEPX2, point->y-STEPY2, z1);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z1);
                glVertex3i(point->x+STEPX2, point->y+STEPY2, z1);
                glVertex3i(point->x-STEPX2, point->y+STEPY2, z1);

                glVertex3i(point->x+STEPX2, point->y-STEPY2, z1);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y+STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y+STEPY2, z1);

                glVertex3i(point->x-STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y+STEPY2, z2);
                glVertex3i(point->x-STEPX2, point->y+STEPY2, z2);

                glVertex3i(point->x-STEPX2, point->y-STEPY2, z1);
                glVertex3i(point->x-STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x-STEPX2, point->y+STEPY2, z2);
                glVertex3i(point->x-STEPX2, point->y+STEPY2, z1);

                glVertex3i(point->x-STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z1);
                glVertex3i(point->x-STEPX2, point->y-STEPY2, z1);

                glVertex3i(point->x-STEPX2, point->y+STEPY2, z1);
                glVertex3i(point->x+STEPX2, point->y+STEPY2, z1);
                glVertex3i(point->x+STEPX2, point->y-STEPY2, z2);
                glVertex3i(point->x-STEPX2, point->y-STEPY2, z2);

                glEnd();
			}
		}
	}

    glFlush();
    SDL_GL_SwapBuffers();
}

void setColor(QString codeCoul) {
    if(codeCoul == "15") {
        glColor3ub(255, 255, 255);
    }else if(codeCoul == "4") {
        glColor3ub(255, 0, 0);
    }else if(codeCoul == "1") {
        glColor3ub(0, 0, 255);
    }else if(codeCoul == "78") {
        glColor3ub(246, 212, 179);
    }else if(codeCoul == "70") {
        glColor3ub(88, 42, 18);
    }else {
        glColor3ub(0, 0, 0);
    }
}
