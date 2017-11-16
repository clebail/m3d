#include <QFile>
#include <QtDebug>
#include <QStringList>
#include <QDir>

#include <common.h>

int main(void) {
    QString ldrFileName = LDRS_FOLDER+"result.ldr";
	QFile ldrFile(ldrFileName);
	
	if(ldrFile.open(QIODevice::WriteOnly)) {
		int i;
		QTextStream ldrStream(&ldrFile);
	
		for(i=0;i<NB_PAS;i++) {
            QString txtFileName = "../txts/"+QString::number(i+1).rightJustified(4, '0')+".txt";
			QFile txtFile(txtFileName);

			if(txtFile.open(QIODevice::ReadOnly)) {
				QTextStream txtStream(&txtFile);

				while(!txtStream.atEnd()) {
					QString line = txtStream.readLine();
					QStringList fields = line.split(";");
					QString ldrLine = "1 %1 %2 %3 %4 1 0 0 0 1 0 0 0 1 3005.DAT\n";
					
					int x = fields[0].toInt() / STEPX * 20;
					int z = fields[1].toInt() / STEPZ * 20;
					
					ldrStream << ldrLine.arg(fields[2]).arg(QString::number(x)).arg(-(NB_PAS-i)*24).arg(QString::number(z));
				}

				txtFile.close();
			}
			//break;
		}
		
		ldrFile.close();
	}

	return 0;
}
