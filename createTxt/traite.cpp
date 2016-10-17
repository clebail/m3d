#include <QImage>
#include <QtDebug>
#include <QFile>
#include <math.h>

#include <common.h>

int main(void) {
	int i,j,x,y;
	SPoint points[NB_PAS][NB_IMAGE];
	
	for(i=0;i<NB_IMAGE;i++) {
		QImage image;
		int n;
		double angle = ((double)i)*2.0*PI/((double)NB_IMAGE);
		double sinA = sin(angle);
		double cosA = cos(angle);
		
		image.load("../images/"+QString::number(i+1)+".jpg");
		for(y=n=0;y<image.size().height();y+=STEPY,n++) {
			SPoint *p = &points[n][i];
			
			p->x = 0;
			p->y = 0;
			p->coul = 0;
			
			for(x=0;x<image.size().width();x++) {
				unsigned rgb = image.pixel(x, y);
				unsigned r = (rgb & 0x00FF0000) >> 16;
				unsigned g = (rgb & 0x0000FF00) >> 8;
				unsigned b = (rgb & 0x000000FF);
				
				if(r > 50 && g > 50 && b > 50) {
					SPoint *p = &points[n][i];
					
					
					p->x = (MIDDLE - x) * cosA;
					p->y = (MIDDLE - x) * sinA;
					p->coul = 'B';
					
					break;
				}
			}
		}
	}
	
	for(j=0;j<NB_PAS;j++) {
		QString carres = "";
		QString fileName = "../txts/"+QString::number(j+1)+".txt";
		QFile file(fileName);

		if(file.open(QIODevice::WriteOnly)) {
			QTextStream stream(&file);
			int pX=1000;
			int pY=1000;

			for(i=0;i<NB_IMAGE;i++) {
				SPoint *p = &points[j][i];
				QString carre="%1;%2;B\n";
		
				x = (p->x/STEPX)*STEPX;
				if(p->x < 0) {
					x -= p->x % STEPX != 0 ? STEPX : 0;
				}
				
				y = (p->y/STEPY)*STEPY;
				if(p->y < 0) {
					y -= p->y % STEPY != 0 ? STEPY : 0;
				}

				if(x != pX || y != pY) {
					carres += carre.arg(x).arg(y);
				}

				pX = x;
				pY = y;
			}
			stream << carres;

			file.close();
		}
	}

	
	return 0;
}
