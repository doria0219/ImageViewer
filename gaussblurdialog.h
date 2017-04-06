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

private:
    Ui::GaussBlurDialog *ui;
};

#endif // GAUSSBLURDIALOG_H
