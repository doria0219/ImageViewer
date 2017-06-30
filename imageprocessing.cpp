#include "imageprocessing.h"

ImageProcessing::ImageProcessing()
{

}

/**
 * grb ==> gray
 */
QImage ImageProcessing::rbg2gray(const QImage & img)
{
//    QImage ret(img);
//    for(int i = 0; i< ret.width(); i++){
//        for(int j = 0; j < ret.height(); j++){
//            QRgb rgb = img.pixel(i, j);

//            int grayValue = qGray(rgb);
//            ret.setPixelColor(i, j, qRgb(grayValue, grayValue, grayValue));
//        }
//    }

//    return ret;

    return Matrix<int>::toQImage(Matrix<int>::fromQImage(img, 'h'));
}

/**
 * reverse the color
 */
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

/**
 * log transform for a picture
 */
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

/**
 * histequ for gray
 */
QImage ImageProcessing::histEquilibrium(const QImage & img){

    QImage ret(img);

    int width = img.width();
    int height = img.height();
    int N = width * height;

    // calculate gray-sum of every gray-level
    int hist[256];
    std::fill(hist, hist + 256, 0);

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            hist[qGray(img.pixel(i, j))]++;
        }
    }

    int map[256];
    double sum = 0;
    for(int i = 0; i < 256; i++){
        sum += hist[i];
        map[i] = round(sum / N * 255);
    }

    // map
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int g = map[qGray(img.pixel(i, j))];
            ret.setPixel(i, j, qRgb(g, g, g));
        }
    }
    return ret;
}

/**
 * histequ for rgb
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
 * histequ for rgb
 * too slow for using
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
 * histequ in HSI
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
 * RGB ==> HSI
 */
HSI ImageProcessing::Rgb2Hsi(const QRgb rgb){

    HSI hsi;

    // nromalization
    double R = qRed(rgb) / 255.0;
    double G = qGreen(rgb) / 255.0;
    double B = qBlue(rgb) / 255.0;

    // get the range
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
 * HSI ==> RGB
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
 * Hue ==> Rgb
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
 * gauss blur
 */
QImage ImageProcessing::gaussBlurFilter(const QImage & img, const int filterSize, const double sigma, QString patten){

    // calculate the gauss filter kernel
    Matrix<double> kernel = ImageProcessing::computeGaussFilter(filterSize, sigma);

    return ImageProcessing::linearSpacialFilter(img, kernel, patten);
}

/**
 * bilateral filter
 */
QImage ImageProcessing::bilateralFilter(const QImage & img, const int filterSize, const double sigma, const double anotherSigma){

    // calculate the gauss-filter kernel
    Matrix<double> gaussFilter = ImageProcessing::computeGaussFilter(filterSize, sigma);
    // get the distances of every level of bilateral filter
    Matrix<double> bilateralPar = ImageProcessing::computeEveryDistance(anotherSigma);

    // padding
    Matrix<int> proImg = Matrix<int>::fromQImage(ImageProcessing::zeroPadding(img, filterSize, filterSize), 'h');
    Matrix<int> retImg(img.height(), img.width(), 0);

    for(int i = filterSize/2; i < proImg.getNRow() - filterSize/2; i++){
        for(int j = filterSize/2; j < proImg.getNCol() - filterSize/2; j++){

            Matrix<double> curKernel(filterSize, filterSize, 0);
            for(int ii = -filterSize/2; ii <= filterSize/2; ii++){
                for(int jj = -filterSize/2; jj <= filterSize/2; jj++){

                    curKernel(ii + filterSize/2, jj + filterSize/2) = gaussFilter(ii + filterSize/2, jj + filterSize/2) *
                            bilateralPar(0, abs(proImg(i, j) - proImg(i+ii, j+jj)));
                }
            }

            ImageProcessing::filterNormalization(curKernel);
            retImg(i - filterSize/2, j - filterSize/2) = ImageProcessing::getBlockResult(proImg, i, j, curKernel);
        }
    }
    return Matrix<int>::toQImage(retImg);
}

/**
 * compute dis for bilateral filter
 */
Matrix<double> ImageProcessing::computeEveryDistance(const double anotherSigma){

    Matrix<double> dis(1, 256, 0);

    for(int i = 0; i < 255; i++){
        dis(0, i) = exp(- ((i * i) / (2 * anotherSigma * anotherSigma)));
    }
    return dis;
}

/**
 * compute the gauss filter paremeter
 */
Matrix<double> ImageProcessing::computeGaussFilter(const int filterSize, const double sigma){

    Matrix<double> kernel(filterSize, filterSize, 0);
    for(int i = 0; i < filterSize; i++){
        for(int j = 0; j < filterSize; j++){

            double x = (i - filterSize/2) * (i - filterSize/2);
            double y = (j - filterSize/2) * (j - filterSize/2);

            kernel(i, j) = (exp(-((x + y) / 2 / sigma / sigma)));
        }
    }

    double sum = 0;
    for(int i = 0; i < filterSize; i++){
        for(int j = 0; j < filterSize; j++){
            sum += kernel(i, j);
        }
    }

    for(int i = 0; i < filterSize; i++){
        for(int j = 0; j < filterSize; j++){
            kernel(i, j) /= sum;
        }
    }

    return kernel;
}

/**
 * median filter
 * expand && corrosion
 */
QImage ImageProcessing::medianFilter(const QImage & img, const int size, const QString filterPatten, const QString colorPatten){

    QImage padImg = ImageProcessing::zeroPadding(img, size, size);

    QImage ret(img);

    for(int i = size/2; i < img.width(); i++){
        for(int j = size/2; j < img.height(); j++){

            if(colorPatten == "Gray"){
                std::vector<int> block;
                for(int ii = -size/2; ii <= size/2; ii++){
                    for(int jj = -size/2; jj <= size/2; jj++){
                        block.push_back(qGray(padImg.pixel(i+ii, j+jj)));
                    }
                }
                std::sort(block.begin(), block.end());

                int gray;
                if(filterPatten == "median"){
                    gray = block[block.size()/2];
                }else if(filterPatten == "expand"){
                    gray = block[block.size() - 1];
                }else if(filterPatten == "corrosion"){
                    gray = block[0];
                }
                ret.setPixel(i - size/2, j - size/2, qRgb(gray, gray, gray));

            }else if(colorPatten == "RGB"){
                std::vector<int> blockR;
                std::vector<int> blockG;
                std::vector<int> blockB;
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
                    red = blockR[blockR.size()/2];
                    green = blockG[blockG.size()/2];
                    blue = blockB[blockB.size()/2];
                }else if(filterPatten == "expand"){
                    red = blockR[blockR.size() - 1];
                    green = blockG[blockG.size() - 1];
                    blue = blockB[blockB.size() - 1];
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
 * median filter by matrix
 * expand && corrosion
 */
QImage ImageProcessing::medianFilterByMatrix(const QImage & img, const int size, const QString filterPatten, const QString colorPatten){

    // no use
}

/**
 * geometry translate
 * rotation
 */
QImage ImageProcessing::getmetryRotate(const QImage &img, Matrix<double> matrix)
{
    bool isInversed;
    Matrix<double> inversedMat = Matrix<double>::inverseMatrix(matrix, &isInversed);
    if(!isInversed){
        QImage ret(img);
        return ret;
    }

    // calculate the range/size of the new picture
    // the four
    int x1 = 0, y1 = 0;
    int x2 = img.width(), y2 = 0;
    int x3 = 0, y3 = img.height();
    int x4 = img.width(), y4 = img.height();

    // coordinates of four vertex
    int tx1, ty1, tx2, ty2, tx3, ty3, tx4, ty4;
    Matrix<int>::map(matrix, x1, y1, &tx1, &ty1);
    Matrix<int>::map(matrix, x2, y2, &tx2, &ty2);
    Matrix<int>::map(matrix, x3, y3, &tx3, &ty3);
    Matrix<int>::map(matrix, x4, y4, &tx4, &ty4);

//    QMatrix m = QMatrix(0.8, 0.5, -0.5, 0.8, 0, 0);
//    QMatrix im = m.inverted();
//    m.map(x1, y1, &tx1, &ty1);
//    m.map(x2, y2, &tx2, &ty2);
//    m.map(x3, y3, &tx3, &ty3);
//    m.map(x4, y4, &tx4, &ty4);

    // get the range of new picture
    int maxX = tx1>tx2?(tx1>tx3?(tx1>tx4?tx1:tx4):(tx3>tx4?tx3:tx4)):(tx2>tx3?(tx2>tx4?tx2:tx4):(tx3>tx4?tx3:tx4));
    int minX = tx1<tx2?(tx1<tx3?(tx1<tx4?tx1:tx4):(tx3<tx4?tx3:tx4)):(tx2<tx3?(tx2<tx4?tx2:tx4):(tx3<tx4?tx3:tx4));
    int maxY = ty1>ty2?(ty1>ty3?(ty1>ty4?ty1:ty4):(ty3>ty4?ty3:ty4)):(ty2>ty3?(ty2>ty4?ty2:ty4):(ty3>ty4?ty3:ty4));
    int minY = ty1<ty2?(ty1<ty3?(ty1<ty4?ty1:ty4):(ty3<ty4?ty3:ty4)):(ty2<ty3?(ty2<ty4?ty2:ty4):(ty3<ty4?ty3:ty4));
    // the size of new picture
    int height = maxY - minY;
    int width = maxX - minX;
    // translation(平移量), the picture rotate taking the origin as the center
    int deltaX = tx1 - minX;
    int deltaY = ty1 - minY;

    QImage ret(width, height, QImage::Format_RGB32);
    for(int i = -deltaX ; i < ret.width()-deltaX; i++){
        for(int j = -deltaY; j < ret.height()-deltaY; j++){
            // (x, y) is the origenal position of current point
            double x, y;

            // map: new position (i, j) ==> origenal position(x, y)
            // im.map(i, j, &x, &y);
            Matrix<double>::map(inversedMat, i, j, &x, &y);

            if(x >= 0 && x < img.width() - 1 && y >= 0 && y < img.height() - 1){
                ret.setPixel(i + deltaX, j + deltaY, ImageProcessing::bilinearInterpolation(x, y, img));
                // ret.setPixel(i + deltaX, j + deltaY, ImageProcessing::nearestInterpolation(x, y, img));
            }else{
                ret.setPixel(i + deltaX, j + deltaY, qRgb(255, 255, 255));
            }
        }
    }

    return ret;
}

/**
 * gauss low/hight pass filter
 * procssing in frequency
 */
QImage ImageProcessing::gaussLPFilter(const QImage &img, const int d, const int sigma, const char patten)
{

    // patten :
    //         'l' : low pass
    //         'h' : high pass

    /**
     * Code in Matlab:
     *
     *      im = imread('pictures/test2.jpg');
     *      im = rgb2gray(im);
     *
     *      f = fftshift(fft2(im));
     *      % Do your filter here
     *      i = abs(ifft2(fftshift(f)));
     *      figure, imshow(i, []);
     *
     *  rgb2gray ==> fft2d ==> fftshift ==> filter ==> fftshift ==> ifft2d ==> abs ==> normalization.
     *
     */

    // fft2
    Matrix<int> im = Matrix<int>::fromQImage(img, 'h');
    Matrix<std::complex<double> > f = fft2d(im, im.getNRow(), im.getNCol());
    Matrix<std::complex<double> >::fftshift(f);

    // gauss filter
    Matrix<double> filter(f.getNRow(), f.getNCol(), 0);
    int width = filter.getNCol();
    int height = filter.getNRow();
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            double dis = (width/2-j)*(width/2-j) + (height/2-i)*(height/2-i);
            if(dis <= d * d){
                if(patten == 'l'){
                    filter(i, j) = exp((-1 * dis)/(2 * sigma * sigma));
                }else if(patten == 'h'){
                    filter(i, j) = 1- exp((-1 * dis)/(2 * sigma * sigma));
                }
            }else{
                if(patten == 'l'){
                    filter(i, j) = 0;
                }else if(patten == 'h'){
                    filter(i, j) = 1;
                }
            }
        }
    }

    // multiplication
    Matrix<std::complex<double> > multiRes = Matrix<double>::multiplication(f, filter);

    // ifft2
    Matrix<std::complex<double> >::fftshift(multiRes);
    Matrix<std::complex<double> > ifftRes = ifft2d(multiRes, multiRes.getNRow(), multiRes.getNCol());
    Matrix<double> absRes = Matrix<int>::abs4complex(ifftRes);
    Matrix<int> res = Matrix<int>::normalization(absRes);

    // cut
    return Matrix<int>::toQImage(res.subMatrix(0, img.height(), 0, img.width()));
}

/**
 * nearest interpolation
 */
QRgb ImageProcessing::nearestInterpolation(double x, double y, const QImage & img){

    return img.pixel((int)(x + 0.5), (int)(y + 0.5));
}

/**
 * bilinear interpolation
 */
QRgb ImageProcessing::bilinearInterpolation(double x, double y, const QImage & img){

    int r = 0, g = 0, b = 0;

    // four integer point near (x, y)
    QRgb rgb11 = img.pixel((int)x, (int)y);
    QRgb rgb12 = img.pixel((int)x+1, (int)y);
    QRgb rgb21 = img.pixel((int)x, (int)y+1);
    QRgb rgb22 = img.pixel((int)x+1, (int)y+1);

    /**
     * bilinear interpolation by direct calculate
     *
     * g(x, y) = ((1 - dx)*f(2, 1) + dx*f(2,2)) * (1 - dy) + ((1 - dx)*f(1, 1) + dx*f(1, 2)) * dy
     * dx = x - (int)x, dy = y - (int)y
     *
     */
    r = ((1 - x + (int)x) * qRed(rgb21) + (x - (int)x) * qRed(rgb22)) * (1 - y + (int)y) + ((1 - x + (int)x) * qRed(rgb11) + (x - (int)x) * qRed(rgb12)) * (y -(int)y);
    g = ((1 - x + (int)x) * qGreen(rgb21) + (x - (int)x) * qGreen(rgb22)) * (1 - y + (int)y) + ((1 - x + (int)x) * qGreen(rgb11) + (x - (int)x) * qGreen(rgb12)) * (y -(int)y);
    b = ((1 - x + (int)x) * qBlue(rgb21) + (x - (int)x) * qBlue(rgb22)) * (1 - y + (int)y) + ((1 - x + (int)x) * qBlue(rgb11) + (x - (int)x) * qBlue(rgb12)) * (y -(int)y);


    /**
     * bilinear interpolation by matrix map
     *
     * g(x, y) = [1-x  x] 「 f(2, 1)  f(1, 1) | 「 1 - y |
     *                    | f(2, 2)  f(1, 2) 」 |   y   」
     *
     *  this method doesn't work. I dont know why. Maybe there are sth wrong with my using of matrix.map .
     */
//    Matrix<double> m(3, 3, 0);
//    int res1, res2;
//    m.setMatrix(qRed(rgb21), qRed(rgb11), qRed(rgb22), qRed(rgb12), 0, 0);
//    Matrix<int>::map(m, (int)x + 1 - x, x - (int)x, &res1, &res2);
//    r  = res1 * ((int)y + 1 - y) + res2 * (y - (int)x);

//    m.setMatrix(qGreen(rgb21), qGreen(rgb11), qGreen(rgb22), qGreen(rgb12), 0, 0);
//    Matrix<int>::map(m, (int)x + 1 - x, x - (int)x, &res1, &res2);
//    g  = res1 * ((int)y + 1 - y) + res2 * (y - (int)x);

//    m.setMatrix(qBlue(rgb21), qBlue(rgb11), qBlue(rgb22), qBlue(rgb12), 0, 0);
//    Matrix<int>::map(m, (int)x + 1 - x, x - (int)x, &res1, &res2);
//    b  = res1 * ((int)y + 1 - y) + res2 * (y - (int)x);

    return QRgb(qRgb(r, g, b));
}

/**
 * linear spatial fiter
 */
QImage ImageProcessing::linearSpacialFilter(const QImage & img, const Matrix<double> filter, QString patten){

    QImage ret(img);

    int nRow = filter.getNRow();
    int nCol = filter.getNCol();
    if(nRow * nCol % 2 == 0){
        return ret;
    }

    // padding with zero
    QImage paddedImage = ImageProcessing::zeroPadding(ret, nCol, nRow);

    return ImageProcessing::filterImage(paddedImage, filter, patten);
}

/**
 * filter
 */
QImage ImageProcessing::filterImage(const QImage & img, const Matrix<double> filter, const QString patten){

//    QImage ret = QImage(img.width() - nCol + 1, img.height() - nRow + 1, img.format());

//    // int normalRatio = ImageProcessing::filterNormalization(vec, nCol);

//    for(int i = nCol/2; i < img.width() - nCol/2 ; i++){
//        for(int j = nRow/2; j < img.height() - nRow/2; j++){

//            if(patten == "RGB"){
//                // cumpute every block
//                int blockR = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'r');
//                int blockG = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'g');
//                int blockB = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'b');

//                ret.setPixel(i - nCol/2, j - nRow/2, qRgb(blockR, blockG, blockB));
//            }else if(patten == "Gray"){
//                int blockY = ImageProcessing::getBlockResult(img, i, j, vec, nCol, nRow, 'y');

//                ret.setPixel(i - nCol/2, j - nRow/2, qRgb(blockY, blockY, blockY));
//            }else{
//                qDebug() << "patten error!";
//            }
//        }
//    }

//    return ret;


    // convolution & normalization
    if(patten == "RGB"){
        Matrix<double> tempMatR = Matrix<double>::convolution(Matrix<double>::fromQImage(img, 'r'), filter);
        Matrix<double> tempMatG = Matrix<double>::convolution(Matrix<double>::fromQImage(img, 'g'), filter);
        Matrix<double> tempMatB = Matrix<double>::convolution(Matrix<double>::fromQImage(img, 'b'), filter);
        Matrix<int> R = Matrix<int>::normalization(tempMatR);
        Matrix<int> G = Matrix<int>::normalization(tempMatG);
        Matrix<int> B = Matrix<int>::normalization(tempMatB);
        return Matrix<int>::toQImage(R, G, B);
    }else if(patten == "Gray"){
        Matrix<double> tempMat = Matrix<double>::convolution(Matrix<double>::fromQImage(img, 'h'), filter);
        Matrix<int> H = Matrix<int>::normalization(tempMat);
        return Matrix<int>::toQImage(H);
    }
}

/**
 * calculate the result of one block
 */
int ImageProcessing::getBlockResult(const Matrix<int> & img, int i, int j, Matrix<double> filter){

    double sum = 0;

    int nCol = filter.getNCol();
    int nRow = filter.getNRow();

    for(int x = 0; x < nRow; x++){
        for(int y = 0; y < nCol; y++){
            sum += img(i- nRow/2 + x, j- nCol/2 + y) * filter(x, y);
        }
    }

    return (int)sum;
}


/**
 * zero padding
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
 * repear padding
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
 * mirror padding
 * 这个镜像真乃神来之笔
 * 打死我也不会改了
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
 * filter normalization
 */
void ImageProcessing::filterNormalization(Matrix<double> & mat){

    double sum = 0;
    for(int i = 0; i < mat.getNRow(); i++){
        for(int j = 0; j < mat.getNCol(); j++){
            sum += mat(i, j);
        }
    }

    // for sharpening filter，the sum is 0
    if(sum == 0){
        return;
    }

    for(int i = 0; i < mat.getNRow(); i++){
        for(int j = 0; j < mat.getNCol(); j++){
            mat(i, j) /= sum;
        }
    }
}

/**
 * calculate the r,g,b,a of QRgb by self
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
