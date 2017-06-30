#include "logtransformationdlg.h"
#include "ui_logtransformationdlg.h"

LogTransformationDlg::LogTransformationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogTransformationDlg)
{
    ui->setupUi(this);
}

LogTransformationDlg::~LogTransformationDlg()
{
    delete ui;
}

void LogTransformationDlg::on_buttonBox_accepted()
{
    bool isOk;
    double c = ui->lineEdit->text().toDouble(&isOk);

    if(isOk){
        emit confirmed(c);
    }
}
