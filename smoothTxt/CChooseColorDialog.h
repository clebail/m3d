//-----------------------------------------------------------------------------------------------
#ifndef __CCHOOSECOLORDIALOG_H__
#define __CCHOOSECOLORDIALOG_H__
//-----------------------------------------------------------------------------------------------
#include <QDialog>
#include "ui_CChooseColorDialog.h"
//-----------------------------------------------------------------------------------------------
class CChooseColorDialog : public QDialog, private Ui::CChooseColorDialog {
    Q_OBJECT
    public:
        CChooseColorDialog(QWidget *parent = 0);

        QString getColor(void);
    private:
        QString color;
    private slots:
        void lblDoubleClicked(void);
};
//-----------------------------------------------------------------------------------------------
#endif // __CCHOOSECOLORDIALOG_H__
//-----------------------------------------------------------------------------------------------
