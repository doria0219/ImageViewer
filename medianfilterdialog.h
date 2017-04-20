#ifndef MEDIANFILTERDIALOG_H
#define MEDIANFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class MedianFilterDialog;
}

class MedianFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianFilterDialog(QWidget *parent = 0);

    ~MedianFilterDialog();

signals:
    void confirmed(int, QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MedianFilterDialog *ui;

    QString patten;
};

#endif // MEDIANFILTERDIALOG_H
