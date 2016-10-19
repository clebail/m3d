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
		double angle = ((double)i-OFFSET_ANGLE)*2.0*PI/((double)NB_IMAGE);
		double sinA = sin(angle);
		double cosA = cos(angle);
		
		image.load("../images/"+QString::number(i+1)+".jpg");
		for(y=n=0;y<image.size().height();y+=STEPY,n++) {
			SPoint *p = &points[n][i];

			p->coul = '0';
			
			for(x=0;x<image.size().width();x++) {
				unsigned rgb = image.pixel(x, y);
				unsigned r = (rgb & 0x00FF0000) >> 16;
				unsigned g = (rgb & 0x0000FF00) >> 8;
				unsigned b = (rgb & 0x000000FF);
				
				if(r > 70 && g > 70 && b > 70) {
					p->x = (MIDDLE - x) * cosA;
					p->y = (MIDDLE - x) * sinA;
					p->coul = '4'; //ROUGE
					
					image.setPixel(x, y, 0xFFFF0000);
					
					break;
				}
			}
		}
		
		image.save("../images/"+QString::number(i+1)+"_1.jpg");
	}
	
    for(j=0;j<NB_PAS;j++) {
		QString carres = "";
        QString fileName = TXTS_FOLDER+"orig/"+QString::number(j+1).rightJustified(4, '0')+".txt";
		QFile file(fileName);

		if(file.open(QIODevice::WriteOnly)) {
			QTextStream stream(&file);

            for(i=0;i<NB_IMAGE;i++) {
				SPoint *p = &points[j][i];
                QString carre="%1;%2;%3;1\n";
				
                x = (p->x/STEPX)*STEPX;
				if(p->x < 0 && p->x % STEPX != 0) {
					x -= STEPX;
				}
				
				y = (p->y/STEPY)*STEPY;
				if(p->y < 0 && p->y % STEPY != 0) {
					y -= STEPY;
				}

				carres += carre.arg(x).arg(y).arg(p->coul);
			}
			stream << carres;
			
            file.close();
		}
	}

	
	return 0;
}
