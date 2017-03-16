#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include <QStringList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "mymouseevent.h"
#include "imageprocessing.h"
#include "logtransformationdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void reSet();
private slots:
    void on_btn_open_clicked();

    void on_btn_last_clicked();

    void on_btn_next_clicked();

    void on_btn_reset_clicked();

    void on_actionrgb2gray_triggered();

    void on_actionpixelReverse_triggered();

    void on_actionlog_trans_triggered();

    void on_confirmed_accepted(double);

private:
    Ui::MainWindow *ui;

    QFileDialog *fileDialog;
    QGraphicsScene * gs;
    MyMouseEvent * gpi;

    LogTransformationDlg * ltDlg;

    QStringList files;

    QFileInfoList images;

    int curIndex = 0;

    void showImage(QImage img);
    bool getDisplayImage(QImage &) const;
};

#endif // MAINWINDOW_H
