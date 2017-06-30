#include "frequencyfilterdialog.h"
#include "ui_frequencyfilterdialog.h"

FrequencyFilterDialog::FrequencyFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrequencyFilterDialog)
{
    ui->setupUi(this);
}

FrequencyFilterDialog::~FrequencyFilterDialog()
{
    delete ui;
}

void FrequencyFilterDialog::setPatten(char patten)
{
    this->patten = patten;
}

void FrequencyFilterDialog::on_buttonBox_accepted()
{
    bool dOK;
    bool sigmaOK;

    int D = ui->editD->text().toDouble(&dOK);
    double sigma = ui->editSigma->text().toDouble(&sigmaOK);

    if(dOK && sigmaOK){
        emit(confirmed(patten, D, sigma));
    }
}
