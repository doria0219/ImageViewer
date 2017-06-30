#include "geometrytranslatedlg.h"
#include "ui_geometrytranslatedlg.h"

GeometryTranslateDlg::GeometryTranslateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeometryTranslateDlg)
{
    ui->setupUi(this);
}

GeometryTranslateDlg::~GeometryTranslateDlg()
{
    delete ui;
}

void GeometryTranslateDlg::on_buttonBox_accepted()
{
    bool flag[4];

    double m11 = ui->lineEdit->text().toDouble(&flag[0]);
    double m12 = ui->lineEdit_2->text().toDouble(&flag[1]);
    double m21 = ui->lineEdit_4->text().toDouble(&flag[2]);
    double m22 = ui->lineEdit_5->text().toDouble(&flag[3]);

    for(int i = 0; i < 4 ; i++){
        if(!flag[i]){
            return;
        }
    }
    Matrix<double> mat(3, 3, 0);
    mat(0, 0) = m11;
    mat(0, 1) = m12;
    mat(1, 0) = m21;
    mat(1, 1) = m22;
    mat(2, 2) = 1;
    emit(sendMatrix(mat));
}
