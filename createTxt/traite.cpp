#include <QImage>
#include <QtDebug>
#include <QFile>
#include <math.h>
#include <common.h>
#include <stdio.h>

int main(void) {
	int i,j,x,y;
	SPoint points[NB_PAS][NB_IMAGE];
	memset(points, 0, NB_PAS*NB_IMAGE*sizeof(SPoint));
	
	for(i=0;i<NB_IMAGE;i++) {
		QImage image;
		int n;
		double angle = ((double)i)*2.0*PI/((double)NB_IMAGE);
		double sinA = sin(angle);
		double cosA = cos(angle);
		
		image.load("../images/"+QString::number(i+1)+".jpg");
		for(y=n=0;y<image.size().height();y+=STEPY,n++) {
			SPoint *p = &points[n][i];
			
			for(x=0;x<image.size().width();x++) {
				unsigned rgb = image.pixel(x, y);
				unsigned r = (rgb & 0x00FF0000) >> 16;
				unsigned g = (rgb & 0x0000FF00) >> 8;
				unsigned b = (rgb & 0x000000FF);
				
				if(r > 70 && g > 70 && b > 70) {
					p->x = (MIDDLE - x) * cosA;
					p->y = (MIDDLE - x) * sinA;
					p->coul = '4'; //ROUGE
					
					break;
				}
			}
		}
	}
	
	printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
	
	for(j=0;j<NB_PAS;j++) {
		QString carres = "";
		QString fileName = "../txts/"+QString::number(j+1)+".txt";
		QFile file(fileName);

		if(file.open(QIODevice::WriteOnly)) {
			QTextStream stream(&file);
			int pX=1000;
			int pY=1000;
			bool first = true;
			
			printf("<g id='%d'>\n", j);
			printf("<path style='stroke:blue;stroke-width:0.5;fill:none;' d='");

			for(i=0;i<NB_IMAGE;i++) {
				SPoint *p = &points[j][i];
				QString carre="%1;%2;%3\n";
				
				printf("%c %d,%d ", first ? 'M' : 'L', p->x, p->y);
		
				x = (p->x/STEPX)*STEPX;
				if(p->x < 0 && p->x % STEPX != 0) {
					x -= STEPX;
				}
				
				y = (p->y/STEPY)*STEPY;
				if(p->y < 0 && p->y % STEPY != 0) {
					y -= STEPY;
				}

				if(x != pX || y != pY) {
					carres += carre.arg(x).arg(y).arg(p->coul);
				}

				pX = x;
				pY = y;
				
				first = false;
			}
			stream << carres;
			
			printf("z' />\n");
			printf("</g>\n");

			file.close();
		}
	}

	
	return 0;
}
