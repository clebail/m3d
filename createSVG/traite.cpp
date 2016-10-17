#include <QFile>
#include <QtDebug>
#include <QStringList>

#include <common.h>

int main(void) {
	int i;

	for(i=0;i<NB_PAS;i++) {
		QString txtFileName = "../txts/"+QString::number(i+1)+".txt";
		QString svgFileName = "../svgs/"+QString::number(i+1)+".svg";
		QFile txtFile(txtFileName);
		QFile svgFile(svgFileName);

		if(txtFile.open(QIODevice::ReadOnly) && svgFile.open(QIODevice::WriteOnly)) {
			QTextStream txtStream(&txtFile);
			QTextStream svgStream(&svgFile);

			svgStream << "<svg xmlns='http://www.w3.org/2000/svg'>" << endl;

			while(!txtStream.atEnd()) {
				QString line = txtStream.readLine();
				QStringList fields = line.split(";");
				QString carre = "\t\t<rect x='%1' y='%2' width='%3' height='%4' style='stroke:black;fill:%5;' />\n";
				QString cercle = "\t\t<circle cx='%1' cy='%2' r='%3' style='stroke:black;fill:none;' />\n";
				
				svgStream << "\t<g>" << endl;
			   	svgStream << carre.arg(fields[0]).arg(fields[1]).arg(STEPX).arg(STEPX).arg("red");
				svgStream << cercle.arg(fields[0].toInt()+STEPX/2).arg(fields[1].toInt()+STEPY/2).arg(((double)STEPX)*ROND_COEF);
				svgStream << "\t</g>" << endl;
			}
			
			svgStream << "</svg>" << endl;

			txtFile.close();
			svgFile.close();
		}
	}

	return 0;
}
