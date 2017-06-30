#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <QDebug>
#include <QRgb>
#include <QMatrix>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "fft.h"

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
    static QImage linearSpacialFilter(const QImage & img, const Matrix<double> filter, QString patten);
    static QImage gaussBlurFilter(const QImage &img, const int filterSize, const double sigma, QString patten);
    static QImage bilateralFilter(const QImage &img, const int filterSize, const double sigma, const double anotherSigma);
    static QImage medianFilter(const QImage &img, const int size, const QString filterPatten, const QString colorPatten);
    static QImage medianFilterByMatrix(const QImage &img, const int size, const QString filterPatten, const QString colorPatten);
    static QImage getmetryRotate(const QImage &img, Matrix<double> matrix);
    static QImage gaussLPFilter(const QImage &img, const int d, const int sigma ,const char patten);

    static void filterNormalization(Matrix<double> &mat);

private:
    static QImage repeatPadding(const QImage &img, const int nCol, const int nRow);
    static QImage mirrorPadding(const QImage &img, const int nCol, const int nRow);
    static QImage zeroPadding(const QImage &img, const int nCol, const int nRow);

private:

    static QRgb Hsi2Rgb(const HSI hsi);
    static HSI Rgb2Hsi(const QRgb rgb);
    static double Hue2Rgb(double v1, double v2,double vH);

    static int getBlockResult(const Matrix<int> &img, int i, int j, Matrix<double> filter);
    static QImage filterImage(const QImage & img, const Matrix<double> filter, QString patten);
    static Matrix<double> computeGaussFilter(const int filterSize, const double sigma);
    static Matrix<double> computeEveryDistance(const double anotherSigma);
    static QRgb bilinearInterpolation(double x, double y, const QImage &img);
    static QRgb nearestInterpolation(double x, double y, const QImage &img);

    static int getAlpha(QRgb rgb);
    static int getRed(QRgb rgb);
    static int getGreen(QRgb rgb);
    static int getBlue(QRgb rgb);
};

#endif // IMAGEPROCESSING_H
