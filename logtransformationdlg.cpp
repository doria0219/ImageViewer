#include "logtransformationdlg.h"
#include "ui_logtransformationdlg.h"
#include <iostream>
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
    // isOK用于检验QString转double是否成果
    bool * isOk;
    double c = ui->lineEdit->text().toDouble(isOk);

    // std::cout << c << std::endl;
    if(isOk){

        // std::cout << "OK" << std::endl;
        emit confirmed(c);
    }
}
