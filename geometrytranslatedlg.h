#ifndef GEOMETRYTRANSLATEDLG_H
#define GEOMETRYTRANSLATEDLG_H

#include <QDialog>
#include <QDebug>
#include "fft.h"

namespace Ui {
class GeometryTranslateDlg;
}

class GeometryTranslateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GeometryTranslateDlg(QWidget *parent = 0);
    ~GeometryTranslateDlg();

signals:
    void sendMatrix(Matrix<double>);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GeometryTranslateDlg *ui;
};

#endif // GEOMETRYTRANSLATEDLG_H
