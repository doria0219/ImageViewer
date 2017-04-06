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
