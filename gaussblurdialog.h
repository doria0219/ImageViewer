#ifndef GAUSSBLURDIALOG_H
#define GAUSSBLURDIALOG_H

#include <QDialog>

namespace Ui {
class GaussBlurDialog;
}

class GaussBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussBlurDialog(QWidget *parent = 0);
    ~GaussBlurDialog();

signals:
    void confirmed(int, double, QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GaussBlurDialog *ui;

    QString patten;
};

#endif // GAUSSBLURDIALOG_H
