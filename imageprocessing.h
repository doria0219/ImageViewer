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
    static QImage linearSpacialFilter(const QImage &img, const QVector<QVector<double> > vec, const int nCol);
    static int filterNormalization(const QVector<QVector<double> > &vec, const int nCOl);

    static void test();

private:

    static QRgb Hsi2Rgb(const HSI hsi);
    static HSI Rgb2Hsi(const QRgb rgb);
    static double Hue2Rgb(double v1, double v2,double vH);



    static int getAlpha(QRgb rgb);
    static int getRed(QRgb rgb);
    static int getGreen(QRgb rgb);
    static int getBlue(QRgb rgb);
    static QImage zeroPadding(const QImage &img, int nCol, int nRow);
    static int getBlockResult(const QImage &img, int i, int j, QVector<QVector<double> > vec, int nCol, int nRow, const char patten);
    static QImage filterImage(const QImage &img, const QVector<QVector<double> > vec, int nCol, int nRow);
};

#endif // IMAGEPROCESSING_H
