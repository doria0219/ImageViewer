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
#include <QRegExp>
#include "mymouseevent.h"
#include "imageprocessing.h"
#include "logtransformationdlg.h"
#include "spacialfiltercernelinput.h"
#include "gaussblurdialog.h"
#include "bilateralfilterdialog.h"
#include "medianfilterdialog.h"
#include "fft.h"
#include "matrixTemplate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_btn_open_clicked();

    void on_btn_last_clicked();

    void on_btn_next_clicked();

    void on_btn_reset_clicked();

    void on_btn_FFT_clicked();

    void on_actionrgb2gray_triggered();

    void on_actionpixelReverse_triggered();

    void on_actionlog_trans_triggered();

    void on_actionhisteq_triggered();

    void on_actionhist_eq_for_rbg_triggered();

    void on_actionhist_eq_for_hsi_triggered();

    void on_actionLinear_triggered();

    void on_LogTransformationDlg_confirmed(double c);

    void on_SpacialFilterCernelInput_confirmed(QString str, QString patten);

    void on_GaussBlurDialog_confirmed(int filterSize, double sigma, QString patten);

    void on_actionGauss_Blur_triggered();

    void on_actionBilateral_Filter_triggered();

    void on_BilateralFilterDialog_confirmed(int size, double sigma, double anotherSigma);

    void on_actionMedian_Filter_triggered();

    void on_Median_FilterDialog_confirmed(int size, QString patten);

    void on_actionExpand_Filter_triggered();

    void on_actionCorrosion_Filter_triggered();

    void on_actionExpand_Filter_RGB_triggered();

    void on_actionCorrosion_Filter_RGB_triggered();

private:
    Ui::MainWindow *ui;

    QFileDialog *fileDialog;
    QGraphicsScene * gs;
    MyMouseEvent * gpi;

    LogTransformationDlg * ltDlg;
    SpacialFilterCernelInput * sfDlg;
    GaussBlurDialog * gbDlg;
    BilateralFilterDialog * bfDlg;
    MedianFilterDialog * mdfDlg;

    QStringList files;

    QFileInfoList images;

    QRegExp * re;

    int curIndex = 0;

    void showImage(QImage img);
    void reSet();
    bool getDisplayImage(QImage &) const;
    QVector<QVector<double> > getFilterData(QString &str, int &colNum);
    QVector<double> getFilterDataOfEveryLine(QString &str, int &colNum);
};

#endif // MAINWINDOW_H
