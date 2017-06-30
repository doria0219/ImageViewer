#include "bilateralfilterdialog.h"
#include "ui_bilateralfilterdialog.h"

BilateralFilterDialog::BilateralFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BilateralFilterDialog)
{
    ui->setupUi(this);
}

BilateralFilterDialog::~BilateralFilterDialog()
{
    delete ui;
}

void BilateralFilterDialog::on_buttonBox_accepted()
{
    bool sizeOK;
    bool sigmaOK;
    bool anothersigmaOK;

    int size = (int)(ui->sizeInputEdit->text().toDouble(&sizeOK));
    double sigma = ui->sigmaInputEdit->text().toDouble(&sigmaOK);
    double anotherSigma = ui->anothersigmaInputEdit->text().toDouble(&anothersigmaOK);

    if(sizeOK && sigmaOK && anothersigmaOK){
        emit confirmed(size, sigma, anotherSigma);
    }else{
        std::cout << "not confirmed!" << std::endl;
    }
}
