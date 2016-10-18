#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QtDebug>
#include <cstdlib>
#include <common.h>

void dessiner(void);
void setColor(char codeCoul);

int main(void) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70, (double)640/480, 1, 1000);
    glEnable(GL_DEPTH_TEST);

    dessiner();

    for (;;) {
        SDL_WaitEvent(&event);

        switch(event.type) {
            case SDL_QUIT:
            exit(0);
            break;
        }
        dessiner();
    }

    return 0;
}

void dessiner() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(0, 1000, 0, 0, 0, 0, 0 ,0, 1);

    glBegin(GL_POINTS);

    
    
	for(int i=0;i<NB_PAS;i++) {
		QString fileName = "../txts/"+QString::number(i+1)+".txt";
		QFile file(fileName);
		int z = (NB_PAS/2-i)*STEPZ;
		
		if(file.open(QIODevice::ReadOnly)) {
			QTextStream txtStream(&file);

			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");
				
				setColor(fields[2][0].toAscii());
				
				glVertex3i(fields[1].toInt(), fields[0].toInt(), z);
			}
			
			file.close();
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
