#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>

struct HSI{
    double h;
    double s;
    int i;
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

    static void test();
private:

    static QRgb Hsi2Rgb(const HSI hsi);
    static HSI Rgb2Hsi(const QRgb rgb);
    static double Hue2Rgb(double v1, double v2,double vH);

    static int getAlpha(QRgb rgb);
    static int getRed(QRgb rgb);
    static int getGreen(QRgb rgb);
    static int getBlue(QRgb rgb);
};

#endif // IMAGEPROCESSING_H
