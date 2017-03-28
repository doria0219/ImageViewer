#ifndef LOGTRANSFORMATIONDLG_H
#define LOGTRANSFORMATIONDLG_H

#include <QDialog>

namespace Ui {
class LogTransformationDlg;
}

class LogTransformationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogTransformationDlg(QWidget *parent = 0);
    ~LogTransformationDlg();

private:
    Ui::LogTransformationDlg *ui;


signals:
    void confirmed(double);

private slots:
    void on_buttonBox_accepted();
};

#endif // LOGTRANSFORMATIONDLG_H
