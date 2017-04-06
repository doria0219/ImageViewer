#ifndef SPACIALFILTERCERNELINPUT_H
#define SPACIALFILTERCERNELINPUT_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class SpacialFilterCernelInput;
}

class SpacialFilterCernelInput : public QDialog
{
    Q_OBJECT

public:
    explicit SpacialFilterCernelInput(QWidget *parent = 0);
    ~SpacialFilterCernelInput();
signals:
    void confirmed(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SpacialFilterCernelInput *ui;
};

#endif // SPACIALFILTERCERNELINPUT_H
