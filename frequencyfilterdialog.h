#ifndef FREQUENCYFILTERDIALOG_H
#define FREQUENCYFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FrequencyFilterDialog;
}

class FrequencyFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FrequencyFilterDialog(QWidget *parent = 0);
    ~FrequencyFilterDialog();

    void setPatten(char patten);

signals:
    void confirmed(char, int, double);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FrequencyFilterDialog *ui;
    char patten;
};

#endif // FREQUENCYFILTERDIALOG_H
