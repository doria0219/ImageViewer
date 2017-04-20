#ifndef BILATERALFILTERDIALOG_H
#define BILATERALFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class BilateralFilterDialog;
}

class BilateralFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BilateralFilterDialog(QWidget *parent = 0);
    ~BilateralFilterDialog();

signals:
    void confirmed(int, double, double);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BilateralFilterDialog *ui;
};

#endif // BILATERALFILTERDIALOG_H
