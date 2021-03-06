//-----------------------------------------------------------------------------------------------
#ifndef C3DPREVIEW_H
#define C3DPREVIEW_H
//-----------------------------------------------------------------------------------------------
#include <QDialog>
#include <QHash>
#include <QList>
#include <QMenu>
#include "common.h"
#include "ui_C3dPreview.h"
//-----------------------------------------------------------------------------------------------
class C3dPreview : public QDialog, private Ui::C3dPreview {
    Q_OBJECT
    public:
        C3dPreview(QWidget *parent=nullptr, QHash<QString, QList<QList<SPoint *>*>*> *map = nullptr);
        ~C3dPreview(void);
    private:
        QHash<QString, QList<QList<SPoint *>*>*> *map;
        QMenu *lvMenu;
        QList<SFace *> sfMap;
        QStringList colors;
        QListWidget *popupSender;

        void createList(void);
        void computeMap(void);
        QColor getColor(QString coul);
        QString getColorNameFromIdx(QString coul);
        void clearMap(void);
    private slots:
        void on_hsX_valueChanged(int value);
        void on_hsY_valueChanged(int value);
        void on_hsZ_valueChanged(int value);
        void on_lvLayers_itemSelectionChanged(void);
        void on_lvColors_itemSelectionChanged(void);
        void selectedAll(void);
        void selectedNone(void);
        void popupMenu(const QPoint &);
        void on_hsLX_valueChanged(int value);
        void on_hsLY_valueChanged(int value);
};
//-----------------------------------------------------------------------------------------------
#endif // C3DPREVIEW_H
//-----------------------------------------------------------------------------------------------
