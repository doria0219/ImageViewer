#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ltDlg(new LogTransformationDlg),
    sfDlg(new SpacialFilterCernelInput),
    gbDlg(new GaussBlurDialog),
    bfDlg(new BilateralFilterDialog),
    mdfDlg(new MedianFilterDialog),
    gtDlg(new GeometryTranslateDlg),
    ffDlg(new FrequencyFilterDialog)
{
    ui->setupUi(this);

    gpi = new MyMouseEvent();
    gs = new QGraphicsScene(this);

    fileDialog = new QFileDialog(this);

    // cannot connect slots by name
    // connect by self
    connect(ltDlg,SIGNAL(confirmed(double)),this,SLOT(on_LogTransformationDlg_confirmed(double)));
    connect(sfDlg,SIGNAL(confirmed(QString, QString)),this,SLOT(on_SpacialFilterCernelInput_confirmed(QString, QString)));
    connect(gbDlg,SIGNAL(confirmed(int, double, QString)),this,SLOT(on_GaussBlurDialog_confirmed(int, double, QString)));
    connect(bfDlg,SIGNAL(confirmed(int,double,double)),this,SLOT(on_BilateralFilterDialog_confirmed(int,double,double)));
    connect(mdfDlg,SIGNAL(confirmed(int, QString)),this,SLOT(on_Median_FilterDialog_confirmed(int, QString)));
    connect(gtDlg,SIGNAL(sendMatrix(Matrix<double>)),this,SLOT(on_GeometryTranslateDialog_sendMatrix(Matrix<double>)));
    connect(ffDlg,SIGNAL(confirmed(char, int, double)),this,SLOT(on_FrequencyFilterDialog_confirmed(char,int,double)));
}

MainWindow::~MainWindow(){

    delete gpi;
    delete gs;
    delete ui;

    delete ltDlg;
    delete sfDlg;
    delete gbDlg;
    delete bfDlg;
    delete mdfDlg;
    delete gtDlg;
    delete fileDialog;
}

/**
 * button open
 */
void MainWindow::on_btn_open_clicked(){

    if(fileDialog->exec()){
        files = fileDialog->selectedFiles();

        QDir dir = fileDialog->directory();
        // string filter
        QStringList filters;
        filters << "*.jpg" << "*.bmp" << "*.png";
        images = dir.entryInfoList(filters);

        for(int i = 0; i < images.length(); i++){
            if(files[0] == images[i].absoluteFilePath()){
                curIndex = i;
            }
        }

        showImage(QImage(files[0]));
    }
}

/**
 * show the image
 */
void MainWindow::showImage(QImage img){

    if(gpi){
        delete gpi;
        gpi = new MyMouseEvent();
    }

    gpi->setPixmap(QPixmap::fromImage(img));

    gs->addItem(gpi);

    ui->graphicsView->setScene(gs);

    reSet();
}

/**
 * save image
 */
void MainWindow::saveImage()
{
    QImage img;

    if(getDisplayImage(img)){

        QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("*.png;; *.jpg;;"));
        if(filename.isEmpty()){
            return;
        }else{
            if(!(img.save(filename))){
                return;
            }
        }
    }
}

/**
 * get the image displayed
 */
bool MainWindow::getDisplayImage(QImage & img) const
{
    if(gs->items().length() != 0){

        QList<QGraphicsItem *> list = ui->graphicsView->scene()->items();
        QGraphicsPixmapItem * item = (QGraphicsPixmapItem*)list.at(0);
        img = item->pixmap().toImage();
        return true;
    }
    return false;
}

/**
 * button last
 */
void MainWindow::on_btn_last_clicked()
{
    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + --curIndex) % images.length()].absoluteFilePath()));
}

/**
 * button next
 */
void MainWindow::on_btn_next_clicked()
{

    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + ++curIndex) % images.length()].absoluteFilePath()));
}

/**
 * button reset
 */
void MainWindow::on_btn_reset_clicked()
{
    reSet();
}

/**
 * reset the size and position of cur images to adapt to the window
 */
void MainWindow::reSet()
{
    // no image
    if(NULL == gpi){
        return;
    }

    gpi->reSet();

    // QGraphics adapt
    gs->setSceneRect(gpi->boundingRect());
    // graphicsView adapt
    ui->graphicsView->fitInView(gpi->boundingRect(), Qt::KeepAspectRatio);
}

/**
 * button rgb2gray
 */
void MainWindow::on_actionrgb2gray_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::rbg2gray(img);
        showImage(img);
    }
}

/**
 * pixel reverse
 */
void MainWindow::on_actionpixelReverse_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::pixelReverse(img);
        showImage(img);
    }
}

/**
 * log
 */
void MainWindow::on_actionlog_trans_triggered()
{
    ltDlg->exec();
}

/**
 * log
 */
void MainWindow::on_LogTransformationDlg_confirmed(double c)
{

    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::logTransformation(img, c);
        showImage(img);
    }
}

/**
 * filter kernel input window
 */
void MainWindow::on_actionLinear_triggered()
{
    sfDlg->exec();
}

/**
 * gauss blur
 */
void MainWindow::on_actionGauss_Blur_triggered()
{
    gbDlg->exec();
}

/**
 * get parameters from sfDLg and process image
 */
void MainWindow::on_SpacialFilterCernelInput_confirmed(QString str, QString patten){

    int colNum = 0;

    // get the data from str
    std::vector<std::vector<double>> filterData = getFilterData(str, colNum);

    // vector2matrix
    Matrix<double> filter(filterData.size(), colNum, 0);
    for(int i = 0; i < filterData.size(); i++){
        for(int j = 0; j < colNum; j++){
            filter(i, j) = filterData[i][j];
        }
    }
    ImageProcessing::filterNormalization(filter);
//    test for vector2matrix
//    qDebug() << "QVector:    " << filterData;
//    std::cout << "Matirx:" << std::endl << filter << std::endl;

    QImage img;
    if(getDisplayImage(img)){

        img = ImageProcessing::linearSpacialFilter(img, filter, patten);
        showImage(img);
    }
}

void MainWindow::on_GaussBlurDialog_confirmed(int filterSize, double sigma, QString patten){

    QImage img;
    if(getDisplayImage(img)){

        img = ImageProcessing::gaussBlurFilter(img, filterSize, sigma, patten);
        showImage(img);
    }
}

std::vector<std::vector<double> > MainWindow::getFilterData(QString & str, int & colNum){

    std::vector<std::vector<double>> vec;

    // split into lines
    QStringList qsl = str.replace("\n", "").trimmed().split(QRegExp("\\s*;"));

    // first line
    std::vector<double> vecLine = getFilterDataOfEveryLine(qsl[0], colNum);
    vec.push_back(vecLine);

    // from second line
    for(int i = 1; i < qsl.length() && qsl[i] != ""; i++){

        int curColNum = 0;
        std::vector<double> vecLine = getFilterDataOfEveryLine(qsl[i], curColNum);
        if(curColNum == colNum){
            vec.push_back(vecLine);
        }
    }
    return vec;
}

std::vector<double> MainWindow::getFilterDataOfEveryLine(QString &str, int & colNum){

    std::vector<double> vec;

    // split into single item
    QStringList qsl = str.replace("\n","").trimmed().split(QRegExp("\\s*,"));

    // number of current line
    colNum = qsl.length();
    for(int i = 0; i < qsl.length() && qsl[i] != ""; i++){

        double d = (double)qsl.at(i).toDouble();

        vec.push_back(d);
    }
    return vec;
}


/**
 * histeq 4 gray
 */
void MainWindow::on_actionhisteq_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        showImage(ImageProcessing::histEquilibrium(img));
    }
}

/**
 * histeq 4 RGB
 */
void MainWindow::on_actionhist_eq_for_rbg_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        showImage(ImageProcessing::histEquilibriumForRgb(img));
    }
}

/**
 * histeq 4 HSI
 */
void MainWindow::on_actionhist_eq_for_hsi_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        showImage(ImageProcessing::histEquilibriumByHSI(img));
    }
}


void MainWindow::on_actionBilateral_Filter_triggered()
{
    bfDlg->exec();
}

void MainWindow::on_BilateralFilterDialog_confirmed(int size, double sigma, double anotherSigma){

    QImage img;

    if(getDisplayImage(img)){

        showImage(ImageProcessing::bilateralFilter(img, size, sigma, anotherSigma));
    }
}

void MainWindow::on_FrequencyFilterDialog_confirmed(char patten, int d, double sigma)
{
    QImage img;

    if(getDisplayImage(img)){
        showImage(ImageProcessing::gaussLPFilter(img, d, sigma, patten));
    }
}

void MainWindow::on_GeometryTranslateDialog_sendMatrix(Matrix<double> matrix)
{
    QImage img;

    if(getDisplayImage(img)){
        showImage(ImageProcessing::getmetryRotate(img, matrix));
    }
}

void MainWindow::on_actionMedian_Filter_triggered()
{
    mdfDlg->exec();
}

void MainWindow::on_actionRotation_triggered()
{
    gtDlg->exec();
}

void MainWindow::on_Median_FilterDialog_confirmed(int size, QString patten){

    QImage img;

    if(getDisplayImage(img)){

        showImage(ImageProcessing::medianFilter(img, size, "median", patten));
    }
}

void MainWindow::on_actionExpand_Filter_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        const int size = 3;
        showImage(ImageProcessing::medianFilter(img, size, "expand", "Gray"));
    }
}

void MainWindow::on_actionCorrosion_Filter_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        const int size = 3;
        showImage(ImageProcessing::medianFilter(img, size, "corrosion", "Gray"));
    }
}

void MainWindow::on_actionExpand_Filter_RGB_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        const int size = 5;
        showImage(ImageProcessing::medianFilter(img, size, "expand", "RGB"));
    }
}

void MainWindow::on_actionCorrosion_Filter_RGB_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        const int size = 5;
        showImage(ImageProcessing::medianFilter(img, size, "corrosion", "RGB"));
    }
}

void MainWindow::on_actionFFT_triggered()
{
    QImage img;

    // Matrix<int>::test4MatrixTranspose();

    if(getDisplayImage(img)){

        Matrix<int> g = Matrix<int>::fromQImage(img, 'h');
        Matrix<std::complex<double> > ff = fft2d(g, g.getNRow(), g.getNCol());
        Matrix<double> ffta = Matrix<double>::abs4complex(ff);
        Matrix<double> fftl = Matrix<double>::logtranslate(ffta, 1);
        Matrix<double>::fftshift(fftl);
        Matrix<int> fftres = Matrix<int>::normalization(fftl);
        QImage res = Matrix<int>::toQImage(fftres);
        showImage(res);
    }
}

void MainWindow::on_actionsave_image_triggered()
{
    saveImage();
}

void MainWindow::on_actionLP_filter_triggered()
{
    ffDlg->setPatten('l');
    ffDlg->exec();
}

void MainWindow::on_actionHP_filter_triggered()
{
    ffDlg->setPatten('h');
    ffDlg->exec();
}
