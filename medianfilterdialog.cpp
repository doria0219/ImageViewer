#include "medianfilterdialog.h"
#include "ui_medianfilterdialog.h"


MedianFilterDialog::MedianFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianFilterDialog)
{
    ui->setupUi(this);
}

MedianFilterDialog::~MedianFilterDialog()
{
    delete ui;
}

void MedianFilterDialog::on_buttonBox_accepted()
{
    bool ok;

    int size = ui->lineEdit->text().toDouble(&ok);

    if(ok){
        if(ui->rBtnRGB->isChecked()){
            patten = "RGB";
        }else if(ui->rBtnGray->isChecked()){
            patten = "Gray";
        }
        emit(confirmed(size, patten));
    }
}
