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
#include <QMatrix>
#include "mymouseevent.h"
#include "imageprocessing.h"
#include "logtransformationdlg.h"
#include "spacialfiltercernelinput.h"
#include "gaussblurdialog.h"
#include "bilateralfilterdialog.h"
#include "medianfilterdialog.h"
#include "frequencyfilterdialog.h"
#include "fft.h"
#include "geometrytranslatedlg.h"

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

    void on_FrequencyFilterDialog_confirmed(char patten, int d, double sigma);

    void on_GeometryTranslateDialog_sendMatrix(Matrix<double>);

    void on_actionMedian_Filter_triggered();

    void on_Median_FilterDialog_confirmed(int size, QString patten);

    void on_actionExpand_Filter_triggered();

    void on_actionCorrosion_Filter_triggered();

    void on_actionExpand_Filter_RGB_triggered();

    void on_actionCorrosion_Filter_RGB_triggered();

    void on_actionRotation_triggered();

    void on_actionFFT_triggered();

    void on_actionsave_image_triggered();

    void on_actionLP_filter_triggered();

    void on_actionHP_filter_triggered();

private:

    Ui::MainWindow *ui;

    QGraphicsScene * gs;
    MyMouseEvent * gpi;

    // dialogs
    QFileDialog *fileDialog;
    LogTransformationDlg * ltDlg;
    SpacialFilterCernelInput * sfDlg;
    GaussBlurDialog * gbDlg;
    BilateralFilterDialog * bfDlg;
    MedianFilterDialog * mdfDlg;
    GeometryTranslateDlg * gtDlg;
    FrequencyFilterDialog * ffDlg;

    QStringList files;
    QFileInfoList images;

    QRegExp * re;

    // cur picture index
    int curIndex = 0;

    void showImage(QImage img);
    void saveImage();
    void reSet();
    bool getDisplayImage(QImage &) const;

    std::vector<std::vector<double> > getFilterData(QString &str, int &colNum);
    std::vector<double> getFilterDataOfEveryLine(QString &str, int &colNum);
};

#endif // MAINWINDOW_H
