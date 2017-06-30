#include "gaussblurdialog.h"
#include "ui_gaussblurdialog.h"

GaussBlurDialog::GaussBlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussBlurDialog)
{
    ui->setupUi(this);
}

GaussBlurDialog::~GaussBlurDialog()
{
    delete ui;
}

void GaussBlurDialog::on_buttonBox_accepted()
{
    bool sizeOK;
    bool sigmaOK;

    int size = (int)(ui->sizeInputEdit->text().toDouble(&sizeOK));
    double sigma = ui->sigmaInputEdit->text().toDouble(&sigmaOK);

    if(sizeOK && sigmaOK){

        if(ui->rBtnRGB->isChecked()){
            patten = "RGB";
        }else if(ui->rBtnGray->isChecked()){
            patten = "Gray";
        }
        emit confirmed(size, sigma, patten);
    }
}
