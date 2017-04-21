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
 * 高斯滤波
 */
QImage ImageProcessing::gaussBlurFilter(const QImage & img, const int filterSize, const double sigma, QString patten){

    QVector<QVector<double>> kernel = ImageProcessing::computeGaussFilter(filterSize, sigma);

    return ImageProcessing::linearSpacialFilter(img, kernel, filterSize, patten);
}


/**
 * 双边滤波
 */
QImage ImageProcessing::bilateralFilter(const QImage & img, const int filterSize, const double sigma, const double anotherSigma){

    QVector<QVector<double>> gaussFIilter = ImageProcessing::computeGaussFilter(filterSize, sigma);

    QVector<double> bilateralPar = ImageProcessing::computeEveryDistance(anotherSigma);

    QImage proImg = ImageProcessing::zeroPadding(img, filterSize, filterSize);
    QImage ret = QImage(img.width(), img.height(), img.format());

    for(int i = filterSize/2; i < proImg.width() - filterSize/2; i++){
        for(int j = filterSize/2; j < proImg.height() - filterSize/2; j++){

            QVector<QVector<double> > curKernel;

            for(int ii = -filterSize/2; ii <= filterSize/2; ii++){
                QVector<double> lineKernel;
                for(int jj = -filterSize/2; jj <= filterSize/2; jj++){

                    lineKernel.push_back(gaussFIilter[ii + filterSize/2][jj + filterSize/2] *
                            bilateralPar[abs(qGray(proImg.pixel(i, j)) -
                            qGray(proImg.pixel(i + ii, j + jj)))]);

                }
                curKernel.push_back(lineKernel);
            }

            ImageProcessing::filterNormalization(curKernel, filterSize);
            int blockGray = ImageProcessing::getBlockResult(proImg, i, j, curKernel, filterSize, filterSize, 'y');

            ret.setPixel(i - filterSize/2, j - filterSize/2, qRgb(blockGray, blockGray, blockGray));
        }
    }

    return ret;

}

QVector<double> ImageProcessing::computeEveryDistance(const double anotherSigma){
    QVector<double> dis;

    for(int i = 0; i < 255; i++){
        dis.push_back(exp(- ((i * i) / (2 * anotherSigma * anotherSigma))));
    }
    return dis;
}

QVector<QVector<double>> ImageProcessing::computeGaussFilter(const int filterSize, const double sigma){

    QVector<QVector<double>> kernel;
    for(int i = 0; i < filterSize; i++){
        QVector<double> line;
        for(int j = 0; j < filterSize; j++){

            double x = (i - filterSize/2) * (i - filterSize/2);
            double y = (j - filterSize/2) * (j - filterSize/2);

            line.push_back(exp(-((x + y) / 2 / sigma / sigma)));
        }
        kernel.push_back(line);
    }

    double sum = 0;
    for(int i = 0; i < filterSize; i++){
        for(int j = 0; j < filterSize; j++){
            sum += kernel[i][j];
        }
    }

    for(int i = 0; i < filterSize; i++){
        for(int j = 0; j < filterSize; j++){
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

/**
 * 中值滤波器
 * 膨胀和腐蚀
 */
QImage ImageProcessing::medianFilter(const QImage & img, const int size, const QString filterPatten, const QString colorPatten){

    QImage padImg = ImageProcessing::zeroPadding(img, size, size);

    QImage ret(img);

    for(int i = size/2; i < img.width(); i++){
        for(int j = size/2; j < img.height(); j++){

            if(colorPatten == "Gray"){
                QVector<int> block;
                for(int ii = -size/2; ii <= size/2; ii++){
                    for(int jj = -size/2; jj <= size/2; jj++){
                        block.push_back(qGray(padImg.pixel(i+ii, j+jj)));
                    }
                }
                std::sort(block.begin(), block.end());

                int gray;
                if(filterPatten == "median"){
                    gray = block[block.length()/2];
                }else if(filterPatten == "expand"){
                    gray = block[block.length() - 1];
                }else if(filterPatten == "corrosion"){
                    gray = block[0];
                }
                ret.setPixel(i - size/2, j - size/2, qRgb(gray, gray, gray));

            }else if(colorPatten == "RGB"){
                QVector<int> blockR;
                QVector<int> blockG;
                QVector<int> blockB;
                for(int ii = -size/2; ii <= size/2; ii++){
                    for(int jj = -size/2; jj <= size/2; jj++){
                        blockR.push_back(qRed(padImg.pixel(i+ii, j+jj)));
                        blockG.push_back(qGreen(padImg.pixel(i+ii, j+jj)));
                        blockB.push_back(qBlue(padImg.pixel(i+ii, j+jj)));
                    }
                }
                std::sort(blockR.begin(), blockR.end());
                std::sort(blockG.begin(), blockG.end());
                std::sort(blockB.begin(), blockB.end());

                int red;
                int green;
                int blue;
                if(filterPatten == "median"){
                    red = blockR[blockR.length()/2];
                    green = blockG[blockG.length()/2];
                    blue = blockB[blockB.length()/2];
                }else if(filterPatten == "expand"){
                    red = blockR[blockR.length() - 1];
                    green = blockG[blockG.length() - 1];
                    blue = blockB[blockB.length() - 1];
                }else if(filterPatten == "corrosion"){
                    red = blockR[0];
                    green = blockG[0];
                    blue = blockB[0];
                }
                ret.setPixel(i - size/2, j - size/2, qRgb(red, green, blue));
            }
        }
    }

    return ret;
}


/**
 * 线性空间滤波
 */
QImage ImageProcessing::linearSpacialFilter(const QImage & img, const QVector<QVector<double>> vec,const int nCol, QString patten){

    QImage ret(img);

    int nRow = vec.length();
    if(nRow * nCol % 2 == 0){
        return ret;
    }

    // 补0
    QImage paddedImage = ImageProcessing::zeroPadding(ret, nCol, nRow);

    // 滤波计算
    QImage filteredImage = ImageProcessing::filterImage(paddedImage, vec, nCol, nRow, patten);

    return filteredImage;
}

/**
 * 滤波
 */
QImage ImageProcessing::filterImage(const QImage & img, const QVector<QVector<double>> vec, int nCol, int nRow, QString patten){

    QImage ret = QImage(img.width() - nCol + 1, img.height() - nRow + 1, img.format());

    // int normalRatio = ImageProcessing::filterNormalization(vec, nCol);

    // 从img非黑色部分开始计算
    for(int i = nCol/2; i < img.width() - nCol/2 ; i++){
        for(int j = nRow/2; j < img.height() - nRow/2; j++){

            if(patten == "RGB"){
                // 直接计算每一块的结果
                int blockR = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'r');
                int blockG = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'g');
                int blockB = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'b');

                ret.setPixel(i - nCol/2, j - nRow/2, qRgb(blockR, blockG, blockB));
            }else if(patten == "Gray"){
                int blockY = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'y');

                ret.setPixel(i - nCol/2, j - nRow/2, qRgb(blockY, blockY, blockY));
            }else{
                qDebug() << "patten error!";
            }
        }
    }

    return ret;
}

/**
 * 获取一块的结果
 */
int ImageProcessing::getBlockResult(const QImage & img, int i, int j, QVector<QVector<double>> vec,int nCol, int nRow, const char patten){

    double sum = 0;

    int indexX = 0;
    for(int x = i - nCol/2; x <= i + nCol/2; x++){
        int indexY = 0;
        for(int y = j - nRow/2; y <= j + nRow/2; y++){

            switch (patten) {
            case 'r':
                sum += qRed(img.pixel(x, y)) * vec[indexX][indexY];
                break;
            case 'g':
                sum += qGreen(img.pixel(x, y)) * vec[indexX][indexY];
                break;
            case 'b':
                sum += qBlue(img.pixel(x, y)) * vec[indexX][indexY];
                break;
            case 'y':
                sum += qGray(img.pixel(x, y)) * vec[indexX][indexY];
            default:
                break;
            }
            indexY++;
        }
        indexX++;
    }
    return (int)sum;
}


/**
 * 补0
 * 测试可行
 */
QImage ImageProcessing::zeroPadding(const QImage & img, const int nCol, const int nRow){

    QImage ret = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());

    for(int i = 0; i < ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){
            if(i >= nCol/2 && i < ret.width() - nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                ret.setPixel(i, j, img.pixel(i - nCol/2, j - nRow/2));
            }else{
                ret.setPixel(i, j, qRgb(0, 0, 0));
            }
        }
    }

    return ret;
}

/**
 * 重复填充
 * 终于不会出问题了
 */
QImage ImageProcessing::repeatPadding(const QImage & img, const int nCol, const int nRow){

    QImage ret = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());

    for(int i = 0; i < ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){

            // 填充左右侧
            if(i < nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                // 最左侧中间
                ret.setPixel(i, j , img.pixel(0, j - nRow/2));
            }else if(i >= ret.width() - nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                // 最右侧中间
                ret.setPixel(i, j, img.pixel(img.width() - 1, j - nRow/2));
            }

            // 填充上下侧
            if(j < nRow/2){
                // 上侧
                if(i >= nCol/2 && i < ret.width() - nCol/2){
                    ret.setPixel(i, j , img.pixel(i - nCol/2, 0));
                }else if(i < nCol/2){
                    ret.setPixel(i, j , img.pixel(0, 0));
                }else if(i >= ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(img.width() - 1, 0));
                }
            }else if(j >= ret.height() - nRow/2){
                // 下侧
                if(i >= nCol/2 && i < ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(i - nCol/2, img.height() - 1));
                }else if(i < nCol/2){
                    ret.setPixel(i, j , img.pixel(0, img.height() - 1));
                }else if(i >= ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(img.width() - 1, img.height() - 1));
                }
            }

            // 中间
            if(i >= nCol/2 && i < ret.width() - nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                ret.setPixel(i, j, img.pixel(i - nCol/2, j - nRow/2));
            }
        }
    }

    return ret;
}

/**
 * 镜像填充
 * 这个镜像真乃神来之笔
 * 这么完美的代码估计也就我能写出来了
 * 要注意不等号
 */
QImage ImageProcessing::mirrorPadding(const QImage & img, const int nCol, const int nRow){

    // 滤波器的大小不能任性
    if(nCol/2 > img.width() || nRow/2 > img.height()){
        qDebug() << "size error!";
        return img;
    }

    QImage ret = QImage(img.width() + nCol - 1, img.height() + nRow - 1, img.format());

    for(int i = 0; i < ret.width(); i++){
        for(int j = 0; j < ret.height(); j++){

            // 填充左右侧
            if(i < nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                // 最左侧中间
                ret.setPixel(i, j , img.pixel(nCol/2 - i, j - nRow/2));
            }else if(i >= ret.width() - nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                // 最右侧中间
                // 这里的代码，一个礼拜后估计我自己也读不懂了，再过一个月，连上帝都读不懂了
                ret.setPixel(i, j, img.pixel(2 * ret.width() - 3 * nCol/2 - i - 1, j - nRow/2));
            }

            // 填充上下侧
            if(j < nRow/2){
                // 上侧
                if(i >= nCol/2 && i < ret.width() - nCol/2){
                    ret.setPixel(i, j , img.pixel(i - nCol/2, nRow/2 - j));
                }else if(i < nCol/2){
                    // r/2 - 1 - i = x - r/2
                    ret.setPixel(i, j , img.pixel(nCol/2 - i - 1, nRow/2 - j - 1));
                }else if(i >= ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(2 * ret.width() - 3 * nCol/2 - i - 1, nRow/2 - j - 1));
                }
            }else if(j >= ret.height() - nRow/2){
                // 下侧
                if(i >= nCol/2 && i < ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(i - nCol/2, 2 * ret.height() - 3 * nCol/2 - j - 1));
                }else if(i < nCol/2){
                    ret.setPixel(i, j , img.pixel(nCol/2 - i - 1, 2 * ret.height() - 3 * nCol/2 - j - 1));
                }else if(i >= ret.width() - nCol/2){
                    ret.setPixel(i, j, img.pixel(2 * ret.width() - 3 * nCol/2 - i - 1, 2 * ret.height() - 3 * nCol/2 - j - 1));
                }
            }

            // 中间
            if(i >= nCol/2 && i < ret.width() - nCol/2 && j >= nRow/2 && j < ret.height() - nRow/2){
                ret.setPixel(i, j, img.pixel(i - nCol/2, j - nRow/2));
            }
        }
    }

    return ret;
}


/**
 * 滤波器归一化
 * 测试可行
 */
void ImageProcessing::filterNormalization(QVector<QVector<double>> & vec, const int nCOl){

    double sum = 0;
    for(int i = 0; i < vec.length(); i++){
        for(int j = 0; j < nCOl; j++){
            sum += vec[i][j];
        }
    }

    // 对于锐化滤波器，算子和为0
    if(sum == 0){
        return;
    }

    for(int i = 0; i < vec.length(); i++){
        for(int j = 0; j < nCOl; j++){
            vec[i][j] /= sum;
        }
    }
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
