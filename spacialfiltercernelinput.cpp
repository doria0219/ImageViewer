#include "spacialfiltercernelinput.h"
#include "ui_spacialfiltercernelinput.h"

SpacialFilterCernelInput::SpacialFilterCernelInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpacialFilterCernelInput)
{
    ui->setupUi(this);
}

SpacialFilterCernelInput::~SpacialFilterCernelInput()
{
    delete ui;
}


void SpacialFilterCernelInput::on_buttonBox_accepted()
{
    QString str = ui->kernelEdit->toPlainText();

    if(str != ""){

        emit(confirmed(str));
    }
}
