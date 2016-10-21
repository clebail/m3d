//-----------------------------------------------------------------------------------------------
#include "CChooseColorDialog.h"
//-----------------------------------------------------------------------------------------------
CChooseColorDialog::CChooseColorDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);

    connect(lblRed, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblWhite, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblBlack, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
}
//-----------------------------------------------------------------------------------------------
QString CChooseColorDialog::getColor(void) {
    return color;
}
//-----------------------------------------------------------------------------------------------
void CChooseColorDialog::lblDoubleClicked(void) {
    color = ((QLabel *)QObject::sender())->text();

    accept();
}
//-----------------------------------------------------------------------------------------------
