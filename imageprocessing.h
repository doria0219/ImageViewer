#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>

class ImageProcessing
{
public:
    ImageProcessing();

    static QImage rbg2gray(const QImage &);
    static QImage pixelReverse(const QImage &);
    static QImage logTransformation(const QImage &, int c);
};

#endif // IMAGEPROCESSING_H
