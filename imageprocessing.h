#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <QDebug>

struct HSI{
    double h;
    double s;
    double i;
};

class ImageProcessing
{
public:

    ImageProcessing();

    static QImage rbg2gray(const QImage &);
    static QImage pixelReverse(const QImage &);
    static QImage logTransformation(const QImage &, int c);
    static QImage histEquilibrium(const QImage &img);
    static QImage histEquilibriumForRgb(const QImage &img);
    static QImage histEquilibriumForRgbNoUse(const QImage &img);    
    static QImage histEquilibriumByHSI(const QImage &img);
    static QImage linearSpacialFilter(const QImage &img, const QVector<QVector<double> > vec, const int nCol, QString patten);
    static QImage gaussBlurFilter(const QImage &img, const int filterSize, const double sigma, QString patten);
    static QImage bilateralFilter(const QImage &img, const int filterSize, const double sigma, const double anotherSigma);
    static QImage medianFilter(const QImage &img, const int size, const QString filterPatten, const QString colorPatten);
    static void filterNormalization(QVector<QVector<double> > &vec, const int nCOl);

    static void test();

private:

    static QRgb Hsi2Rgb(const HSI hsi);
    static HSI Rgb2Hsi(const QRgb rgb);
    static double Hue2Rgb(double v1, double v2,double vH);



    static int getAlpha(QRgb rgb);
    static int getRed(QRgb rgb);
    static int getGreen(QRgb rgb);
    static int getBlue(QRgb rgb);

    static QImage repeatPadding(const QImage &img, const int nCol, const int nRow);
    static QImage mirrorPadding(const QImage &img, const int nCol, const int nRow);
    static QImage zeroPadding(const QImage &img, const int nCol, const int nRow);

    static int getBlockResult(const QImage &img, int i, int j, QVector<QVector<double> > vec, int nCol, int nRow, const char patten);
    static QImage filterImage(const QImage &img, const QVector<QVector<double> > vec, int nCol, int nRow, QString patten);
    static QVector<QVector<double> > computeGaussFilter(const int filterSize, const double sigma);
    static QVector<double> computeEveryDistance(const double anotherSigma);
};

#endif // IMAGEPROCESSING_H
