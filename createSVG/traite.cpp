#include <QFile>
#include <QtDebug>
#include <QStringList>

#include <common.h>

int main(int argc, char **) {
	int i;
    bool drawLego;

    drawLego = (argc == 1);

	for(i=0;i<NB_PAS;i++) {
        QString txtFileName = TXTS_FOLDER+QString::number(i+1).rightJustified(4, '0')+".txt";
        QString svgFileName = SVGS_FOLDER+QString::number(i+1).rightJustified(4, '0')+".svg";
		QFile txtFile(txtFileName);
		QFile svgFile(svgFileName);

		if(txtFile.open(QIODevice::ReadOnly) && svgFile.open(QIODevice::WriteOnly)) {
			QTextStream txtStream(&txtFile);
			QTextStream svgStream(&svgFile);
            int oldId = 0;
            bool first = true;
            int minX=0, maxX=0, minY=0, maxY=0;

			svgStream << "<svg xmlns='http://www.w3.org/2000/svg'>" << endl;

			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");

                if(drawLego) {
                    QString carre = "\t\t<rect x='%1' y='%2' width='%3' height='%4' style='stroke:black;fill:%5;' />\n";
                    QString cercle = "\t\t<circle cx='%1' cy='%2' r='%3' style='stroke:black;fill:none;' />\n";

                    svgStream << "\t<g>" << endl;
                    svgStream << carre.arg(fields[0]).arg(fields[1]).arg(STEPX).arg(STEPX).arg("red");
                    svgStream << cercle.arg(fields[0].toInt()+STEPX/2).arg(fields[1].toInt()+STEPY/2).arg(((double)STEPX)*ROND_COEF);
                    svgStream << "\t</g>" << endl;
                } else {
                    int id = fields[3].toInt();
                    int x = fields[0].toInt(), y = fields[1].toInt();
                    QString coord = "%1 %2,%3 ";

                    if(id != oldId) {
                        if(oldId != 0) {
                            svgStream << "' z />" << endl;
                            svgStream << "\t<g>" << endl;
                        }

                        svgStream << "\t\t<path style='stroke:blue;stroke-width:0.5;fill:none;' d='";
                        first = true;
                    }

                    svgStream << coord.arg(first ? 'M' : 'L').arg(x).arg(y);

                    if(x < minX) {
                        minX = x;
                    }

                    if(x > maxX) {
                        maxX = x;
                    }

                    if(y < minY) {
                        minY = y;
                    }

                    if(y > maxY) {
                        maxY = y;
                    }

                    oldId = id;
                    first = false;
                }
			}

            if(!drawLego) {
                int x, y;

                minX-=STEPX;
                maxX+=STEPX;
                minY-=STEPY;
                maxY+=STEPY;

                svgStream << "' z />" << endl;
                svgStream << "\t<g>" << endl;

                svgStream << "\t<g>" << endl;

                for(y=0;y>=minY;y-=STEPY) {
                    svgStream << QString("\t\t<line x1='%1' y1='%2' x2='%3' y2='%2' style='stroke:red;stroke-width:0.5;' stroke-dasharray='1,5' />").arg(minX).arg(y).arg(maxX) << endl;
                    svgStream << QString("\t\t<text x='%1' y='%2' style='fill:red;'>%2</text>").arg(minX-50).arg(y) << endl;
                }

                for(y=STEPY;y<=maxY;y+=STEPY) {
                    svgStream << QString("\t\t<line x1='%1' y1='%2' x2='%3' y2='%2' style='stroke:red;stroke-width:0.5;' stroke-dasharray='1,5' />").arg(minX).arg(y).arg(maxX) << endl;
                    svgStream << QString("\t\t<text x='%1' y='%2' style='fill:red;'>%2</text>").arg(minX-50).arg(y) << endl;
                }

                for(x=0;x>=minX;x-=STEPX) {
                    svgStream << QString("\t\t<line x1='%1' y1='%2' x2='%1' y2='%3' style='stroke:red;stroke-width:0.5;' stroke-dasharray='1,5' />").arg(x).arg(minY).arg(maxY) << endl;
                    svgStream << QString("\t\t<text x='%1' y='%2' style='fill:red;' transform='rotate(-45, %1, %2)'>%1</text>").arg(x).arg(minY-10) << endl;
                }

                for(x=STEPX;x<=maxX;x+=STEPX) {
                    svgStream << QString("\t\t<line x1='%1' y1='%2' x2='%1' y2='%3' style='stroke:red;stroke-width:0.5;' stroke-dasharray='1,5' />").arg(x).arg(minY).arg(maxY) << endl;
                    svgStream << QString("\t\t<text x='%1' y='%2' style='fill:red;' transform='rotate(-45, %1, %2)'>%1</text>").arg(x).arg(minY-10) << endl;
                }

                svgStream << "\t</g>" << endl;
            }

			svgStream << "</svg>" << endl;

			txtFile.close();
			svgFile.close();
		}
	}

	return 0;
}
