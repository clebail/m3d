#include <QFile>
#include <QtDebug>
#include <QStringList>

#include <common.h>

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

void cleanAll(QList<QHash<int, int>*> *mainList);
void loadDatas(int max, QList<QHash<int, int>*> *mainList, int &maxX, int &maxY, int &maxZ);

int main(int argc, char *argv[]) {
    int max = NB_PAS;
    QList<QHash<int, int>*> *mainList = new QList<QHash<int, int>*>();
    QString binvoxFileName = BINVOX_FOLDER+"out.binvox";
    FILE *binvoxFile;
    QString binvoxColorFileName = binvoxFileName+".color";
    FILE *binvoxColorFile;

    int maxX = 0, maxY = 0, maxZ;

    if(argc != 1) {
        max = QString(argv[1]).toInt();
    }

    loadDatas(max, mainList, maxX, maxY, maxZ);

    int dim = MAX(MAX(maxX, maxY), maxZ);

    if((binvoxFile = fopen(binvoxFileName.toAscii().data(), "wb")) != 0) {
        if((binvoxColorFile = fopen(binvoxColorFileName.toAscii().data(), "w")) != 0) {
            int x, y, z;
            unsigned char value=0, count=0;

            fprintf(binvoxFile, "#binvox 1\n");
            fprintf(binvoxFile, "dim %d %d %d\n", dim, dim, dim);
            fprintf(binvoxFile, "translate 0.0 0.0 0.0\n");
            fprintf(binvoxFile, "scale 1.0\n");
            fprintf(binvoxFile, "data\n");

            for(x=0;x<dim;x++) {
                for(y=0;y<dim;y++) {
                    int key = y*maxX+x;

                    for(z=0;z<dim;z++) {
                        if(x < maxX && y < maxY && z < mainList->size()) {
                            QHash<int ,int> *hash = mainList->at(z);

                            if(hash->contains(key)) {
                                fprintf(binvoxColorFile, "%d;%d;%d;%d\n", x, y, z, hash->value(key));

                                if(value == 1) {
                                    count++;
                                }else {
                                    if(count != 0) {
                                        fwrite(&value, sizeof(unsigned char), 1, binvoxFile);
                                        fwrite(&count, sizeof(unsigned char), 1, binvoxFile);
                                    }

                                    count = 1;
                                    value = 1;
                                }
                            }else {
                                if(value == 0) {
                                    count++;
                                }else {
                                    if(count != 0) {
                                        fwrite(&value, sizeof(unsigned char), 1, binvoxFile);
                                        fwrite(&count, sizeof(unsigned char), 1, binvoxFile);
                                    }

                                    count = 1;
                                    value = 0;
                                }
                            }
                        }else {
                            if(value == 1) {
                                if(count != 0) {
                                    fwrite(&value, sizeof(unsigned char), 1, binvoxFile);
                                    fwrite(&count, sizeof(unsigned char), 1, binvoxFile);

                                    count = 0;
                                }
                                value = 0;
                            }

                            count++;
                        }

                        if(count == 255) {
                            fwrite(&value, sizeof(unsigned char), 1, binvoxFile);
                            fwrite(&count, sizeof(unsigned char), 1, binvoxFile);;

                            count = 0;
                        }
                    }
                }
            }

            if(count != 0) {
                fwrite(&value, sizeof(unsigned char), 1, binvoxFile);
                fwrite(&count, sizeof(unsigned char), 1, binvoxFile);
            }

            fclose(binvoxFile);
            fclose(binvoxColorFile);
        }
    }

    cleanAll(mainList);
}

void cleanAll(QList<QHash<int, int>*> *mainList) {
    int i;

    for(i=0;i<mainList->size();i++) {
        delete mainList->at(i);
    }

    delete mainList;
}

void loadDatas(int max, QList<QHash<int, int>*> *mainList, int &maxX, int &maxY, int &maxZ) {
    int i, minX = 0, minY = 0, plusX, plusY;
    QList<QList<SPoint *> > tmpList;

    maxX = maxY=0;

    for(i=0;i<max;i++) {
        QString txtFileName = TXTS_FOLDER+QString::number(i+1).rightJustified(4, '0')+".txt";
        QFile txtFile(txtFileName);

        if(txtFile.open(QIODevice::ReadOnly)) {
            QTextStream txtStream(&txtFile);
            QList<SPoint *> list;

            while(!txtStream.atEnd()) {
                QString line = txtStream.readLine();
                QStringList fields = line.split(";");
                SPoint *p = new SPoint;

                p->x = fields[0].toInt() / STEPX;
                p->y = fields[1].toInt() / STEPY;
                p->coul = fields[2];

                list.append(p);

                minX=MIN(minX, p->x);
                maxX=MAX(maxX, p->x);
                minY=MIN(minY, p->y);
                maxY=MAX(maxY, p->y);
            }

            tmpList.insert(0, list);

            txtFile.close();
        }
    }

    plusX = abs(minX);
    plusY = abs(minY);

    maxX += plusX+1;
    maxY += plusY+1;
    maxZ = MIN(max, tmpList.size());

    for(i=0;i<maxZ;i++) {
        QHash<int, int> *hash = new QHash<int, int>();
        QList<SPoint *> list = tmpList.at(i);
        int j;

        for(j=0;j<list.size();j++) {
            SPoint *p = list.at(j);

            int key = (p->y+plusY)*maxX+p->x+plusX;

            hash->insert(key, p->coul.toInt());
        }

        mainList->append(hash);
    }
}
