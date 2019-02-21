//-----------------------------------------------------------------------------------------------
#ifndef C3DPREVIEW_H
#define C3DPREVIEW_H
//-----------------------------------------------------------------------------------------------
#include <QDialog>
#include <QHash>
#include <QList>
#include "common.h"
#include "ui_C3dPreview.h"
//-----------------------------------------------------------------------------------------------
class C3dPreview : public QDialog, private Ui::C3dPreview {
    Q_OBJECT
    public:
        C3dPreview(QWidget *parent=nullptr, QHash<QString, QList<QList<SPoint *>*>*> *map = nullptr);
    private:
        QHash<QString, QList<QList<SPoint *>*>*> *map;

        void createList(void);
        QList<SCube *> computeMap(void);
        QColor getColor(QString coul);
    private slots:
        void on_hsX_valueChanged(int value);
        void on_hsY_valueChanged(int value);
        void on_hsZ_valueChanged(int value);
        void on_lvLayers_itemSelectionChanged(void);
};
//-----------------------------------------------------------------------------------------------
#endif // C3DPREVIEW_H
//-----------------------------------------------------------------------------------------------
