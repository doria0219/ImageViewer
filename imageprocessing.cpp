#include "imageprocessing.h"
#include <math.h>

ImageProcessing::ImageProcessing()
{

}

QImage ImageProcessing::rbg2gray(const QImage & img)
{
    QImage ret(img);
    for(int i = 0; i< ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){
            QRgb rgb = img.pixel(i, j);

            int grayValue = qGray(rgb);
            ret.setPixelColor(i, j, qRgb(grayValue, grayValue, grayValue));
        }
    }
    return ret;
}

QImage ImageProcessing::pixelReverse(const QImage & img)
{
    QImage ret(img);
    for(int i = 0; i< ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){
            QRgb rgb = img.pixel(i, j);

            ret.setPixelColor(i, j, qRgb(-qRed(rgb), -qGreen(rgb), -qBlue(rgb)));
        }
    }
    return ret;
}

QImage ImageProcessing::logTransformation(const QImage & img ,int c = 1)
{
    QImage ret(img);

    for(int i = 0; i< ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){

            QRgb rgb = img.pixel(i, j);

            int grayValue = c * log(qGray(rgb) + 1);
            ret.setPixelColor(i, j, qRgb(grayValue, grayValue, grayValue));
        }
    }
    return ret;
}
