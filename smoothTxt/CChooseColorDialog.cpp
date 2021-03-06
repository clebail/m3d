//-----------------------------------------------------------------------------------------------
#include "CChooseColorDialog.h"
//-----------------------------------------------------------------------------------------------
CChooseColorDialog::CChooseColorDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);

    connect(lblRed, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblWhite, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblBlack, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblBlue, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblPink, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblBrown, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblYellow, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
    connect(lblGreen, SIGNAL(doubleClicked(void)), this, SLOT(lblDoubleClicked(void)));
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
