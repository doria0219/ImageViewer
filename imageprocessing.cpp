#include "imageprocessing.h"
#include <algorithm>
#include <iostream>
#include <math.h>

ImageProcessing::ImageProcessing()
{

}

QImage ImageProcessing::rbg2gray(const QImage & img)
{
    // 复制一个新的图像
    QImage ret(img);
    for(int i = 0; i< ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){
            // 获取当前图像的颜色
            QRgb rgb = img.pixel(i, j);

            // 转为灰度
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

            int grayValue = c * log(qGray(rgb)/255.0 + 1) * 255;
            ret.setPixelColor(i, j, qRgb(grayValue, grayValue, grayValue));
        }
    }
    return ret;
}

QImage ImageProcessing::histEquilibrium(const QImage & img){

    QImage ret(img);

    int width = img.width();
    int height = img.height();
    int N = width * height;

    // 统计各级灰度
    int hist[256];
    std::fill(hist, hist + 256, 0);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            hist[qGray(img.pixel(i, j))]++;
        }
    }

    // 累计灰度计算
    int map[256];
    double sum = 0;
    for(int i = 0; i < 256; i++){
        sum += hist[i];
        map[i] = round(sum / N * 255);
    }

    // 映射
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int g = map[qGray(img.pixel(i, j))];
            ret.setPixel(i, j, qRgb(g, g, g));
        }
    }
    return ret;
}

/**
 * 直方图均衡，与灰度算法一样
 */
QImage ImageProcessing::histEquilibriumForRgb(const QImage & img){

    QImage ret(img);

    int width = img.width();
    int height = img.height();
    int N = width * height;

    // count histgram
    int hist[3][256];
    std::fill(hist[0], hist[0] + 3 * 256, 0);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            hist[0][qRed(img.pixel(i, j))]++;
            hist[1][qGreen(img.pixel(i, j))]++;
            hist[2][qBlue(img.pixel(i, j))]++;
        }
    }

    // calculate
    int map[3][256];
    double sum[3] = {0};
    for(int i = 0; i < 256; i++){
        sum[0] += hist[0][i];
        sum[1] += hist[1][i];
        sum[2] += hist[2][i];

        map[0][i] = round(sum[0] / N * 255);
        map[1][i] = round(sum[1] / N * 255);
        map[2][i] = round(sum[2] / N * 255);
    }

    // map the pixel
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int r = map[0][qRed(img.pixel(i, j))];
            int g = map[1][qGreen(img.pixel(i, j))];
            int b = map[2][qBlue(img.pixel(i, j))];
            ret.setPixel(i, j, qRgb(r, g, b));
        }
    }
    return ret;
}

/**
 * RGB直方图均衡，直接调用灰度均衡
 * 时间比较大暂时不用
 */
QImage ImageProcessing::histEquilibriumForRgbNoUse(const QImage & img){

    QImage ret(img);

    QImage imgR(img);
    QImage imgG(img);
    QImage imgB(img);

    for(int i = 0; i < img.width(); i++){
        for(int j = 0; j < img.height(); j++){

            int r = qRed(img.pixel(i, j));
            int g = qGreen(img.pixel(i, j));
            int b = qBlue(img.pixel(i, j));

            imgR.setPixel(i, j, qRgb(r, r, r));
            imgG.setPixel(i, j, qRgb(g, g, g));
            imgB.setPixel(i, j, qRgb(b, b, b));
        }
    }

    imgR = histEquilibrium(imgR);
    imgG = histEquilibrium(imgG);
    imgB = histEquilibrium(imgB);

    for(int i = 0; i < img.width(); i++){
        for(int j = 0; j < img.height(); j++){

            int r = qGray(imgR.pixel(i, j));
            int g = qGray(imgG.pixel(i, j));
            int b = qGray(imgB.pixel(i, j));

            ret.setPixel(i, j, qRgb(r, g, b));
        }
    }

    return ret;
}

/**
 * HSI空间直方图均衡
 */
QImage ImageProcessing::histEquilibriumByHSI(const QImage & img){

    QImage ret(img);

    int width = img.width();
    int height = img.height();
    int N = width * height;

    // count histgram
    int hist[256];
    std::fill(hist, hist + 256, 0);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){

            // 此处I的取值范围为0-1，所以需要乘以255
            hist[(int)(ImageProcessing::Rgb2Hsi(img.pixel(i, j)).i * 255)]++;
        }
    }

    // calculate
    int map[256];
    double sum = 0;
    for(int i = 0; i < 256; i++){
        sum += hist[i];
        map[i] = round(sum / N * 255);
    }

    // map the pixel
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            HSI temp = ImageProcessing::Rgb2Hsi(img.pixel(i,j));
            temp.i = map[(int)(ImageProcessing::Rgb2Hsi(img.pixel(i, j)).i * 255)] / 255.0;

            ret.setPixel(i, j, ImageProcessing::Hsi2Rgb(temp));
        }
    }
    return ret;
}


/**
 * RGB颜色空间转换到HSI中
 */
HSI ImageProcessing::Rgb2Hsi(const QRgb rgb){

    HSI hsi;

    // 归一化
    double R = qRed(rgb) / 255.0;
    double G = qGreen(rgb) / 255.0;
    double B = qBlue(rgb) / 255.0;

    // 求范围
    double min = std::min(std::min(R, G), B);
    double max = std::max(std::max(R, G), B);
    double deltaMax = max - min;

    double H;
    double S;
    double I = (max + min) / 2;

    if (deltaMax == 0 ){
        H = 0;
        S = 0;
    }else{
        H = 0;
        if (I < 0.5)
            S = deltaMax / (max + min);
        else
            S = deltaMax / (2 - max - min);

        double deltaR = (((max - R) / 6.0) + (deltaMax / 2.0)) / deltaMax;
        double deltaG = (((max - G) / 6.0) + (deltaMax / 2.0)) / deltaMax;
        double deltaB = (((max - B) / 6.0) + (deltaMax / 2.0)) / deltaMax;

        if (R == max){
            H = deltaB - deltaG;
        }else if (G == max){
            H = 1 / 3.0 + deltaR - deltaB;
        }else if (B == max){
            H = 2 / 3.0 + deltaG - deltaR;
        }

        if (H < 0)
            H += 1;
        if (H > 1)
            H -= 1;
    }

    hsi.h = H;
    hsi.s = S;
    hsi.i = I;

    return hsi;
}

/**
 * HSI颜色空间转换到RGB中
 */
QRgb ImageProcessing::Hsi2Rgb(const HSI hsi){

    double H = hsi.h;
    double S = hsi.s;
    double I = hsi.i;

    int R = 0;
    int G = 0;
    int B = 0;

    double v1 = 0;
    double v2 = 0;

    if (S == 0) {
        R = I * 255;
        G = I * 255;
        B = I * 255;
    }else {
        if (I < 0.5){
            v2 = I * (1 + S);
        }else{
            v2 = (I + S) - (S * I);
        }

        v1 = 2 * I - v2;

        R = 255 * ImageProcessing::Hue2Rgb(v1, v2, H + 1/3.0);
        G = 255 * ImageProcessing::Hue2Rgb(v1, v2, H);
        B = 255 * ImageProcessing::Hue2Rgb(v1, v2, H - 1/3.0);
    }
    return qRgb(R, G, B);
}

/**
 * Hue2Rgb格式
 */
double ImageProcessing::Hue2Rgb(double v1, double v2,double vH)
{
    if (vH < 0)
        vH += 1;
    if (vH > 1)
        vH -= 1;

    if ((6 * vH) < 1)
        return v1 + (v2 - v1) * 6 * vH;

    if ((2 * vH) < 1)
        return v2;

    if ((3 * vH) < 2)
        return v1 + (v2 - v1) * (2 / 3.0 - vH) * 6;

    return v1;
}

/**
 * 测试HSI与RGB转换是否完全吻合
 */
void ImageProcessing::test(){

    QRgb rgb1 = qRgb(80, 60, 50);
    HSI hsi = ImageProcessing::Rgb2Hsi(rgb1);
    std::cout << hsi.h << "  " << hsi.s << " "<< hsi.i << std::endl;
    QRgb rgb2 = ImageProcessing::Hsi2Rgb(hsi);
    std::cout << qRed(rgb1) << "  " << qBlue(rgb1) << " "<< qGreen(rgb1) << std::endl;
    std::cout << qRed(rgb2) << "  " << qBlue(rgb2) << " "<< qGreen(rgb2) << std::endl;
}


/**
 * 闲来无事，自己手动算一下ARGB分量
 */
int ImageProcessing::getAlpha(QRgb rgb){
    return rgb & 0xff000000;
}

int ImageProcessing::getRed(QRgb rgb){
    return rgb & 0x00ff0000;
}

int ImageProcessing::getGreen(QRgb rgb){
    return rgb & 0x0000ff00;
}

int ImageProcessing::getBlue(QRgb rgb){
    return rgb & 0x000000ff;
}
