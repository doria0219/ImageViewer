#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ltDlg(new LogTransformationDlg),
    sfDlg(new SpacialFilterCernelInput)
{
    ui->setupUi(this);

    gpi = new MyMouseEvent();
    gs = new QGraphicsScene(this);

    fileDialog = new QFileDialog(this);

    // 无法根据名称自动连接信号-槽
    // 暂时先自己连接吧
    connect(ltDlg,SIGNAL(confirmed(double)),this,SLOT(on_LogTransformationDlg_confirmed(double)));
    connect(sfDlg,SIGNAL(confirmed(QString)),this,SLOT(on_SpacialFilterCernelInput_confirmed(QString)));
}

MainWindow::~MainWindow(){

    delete gpi;
    delete gs;
    delete ui;

    delete ltDlg;
    delete fileDialog;
}

/**
 * 打开按钮
 * @brief MainWindow::on_btn_open_clicked
 */
void MainWindow::on_btn_open_clicked(){

    if(fileDialog->exec()){
        files = fileDialog->selectedFiles();

        QDir dir = fileDialog->directory();
        // 字符串过滤
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
 * 显示图片
 * @brief MainWindow::showImage
 * @param img，待显示的图片
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
 * 获取当前正在显示的图片
 * @brief MainWindow::getDisplayImage
 * @param img
 * @return
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
 * 上一张图片
 * @brief MainWindow::on_btn_last_clicked
 */
void MainWindow::on_btn_last_clicked()
{
    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + --curIndex) % images.length()].absoluteFilePath()));
}

/**
 * 下一张图片
 * @brief MainWindow::on_btn_next_clicked
 */
void MainWindow::on_btn_next_clicked()
{

    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + ++curIndex) % images.length()].absoluteFilePath()));
}

/**
 * 重置当前图片的大小和位置
 * @brief MainWindow::on_btn_reset_clicked
 */
void MainWindow::on_btn_reset_clicked()
{
    reSet();
}

/**
 * 重置图片的大小和位置
 * @brief MainWindow::reSet
 */
void MainWindow::reSet()
{
    // 若当前没有图片显示
    if(NULL == gpi){
        return;
    }

    // 调用gpi的初始化函数
    gpi->reSet();

    // QGraphics大小自适应
    gs->setSceneRect(gpi->boundingRect());
    // graphicsView的自适应
    ui->graphicsView->fitInView(gpi->boundingRect(), Qt::KeepAspectRatio);
}

/**
 * RGB转灰度
 * @brief MainWindow::on_actionrgb2gray_triggered
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
 * 图像颜色反转
 * @brief MainWindow::on_actionpixelReverse_triggered
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
 * log 变换按钮
 * @brief MainWindow::on_actionlog_trans_triggered
 */
void MainWindow::on_actionlog_trans_triggered()
{
    // 打开log变换参数输入窗口
    ltDlg->exec();
}

/**
 * 从ltDlg获取参数并且进行log变换
 * @brief MainWindow::on_confirmed_accepted_from_ltDlg
 * @param c
 */
void MainWindow::on_LogTransformationDlg_confirmed(double c)
{
    // qDebug() << "accepted number is:" << c << endl;

    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::logTransformation(img, c);
        showImage(img);
    }
}

/**
 * 打开线性滤波参数输入窗口
 * @brief MainWindow::on_actionLinear_triggered
 */
void MainWindow::on_actionLinear_triggered()
{
    sfDlg->exec();
}

/**
 * 从sfDLg获取参数并且进行线性滤波
 * @brief MainWindow::on_confirmed_accepted_from_sfDlg
 * @param str
 */
void MainWindow::on_SpacialFilterCernelInput_confirmed(QString str){

    // qDebug() << "accepted string is :" << str <<endl;

    int colNum = 0;

    // 获取数据
    // 用于存储矩阵
    QVector<QVector<double>> filterData = getFilterData(str, colNum);

    QImage img;
    if(getDisplayImage(img)){

        img = ImageProcessing::linearSpacialFilter(img, filterData, colNum);
        showImage(img);
    }
}

QVector<QVector<double>> MainWindow::getFilterData(QString & str, int & colNum){

    QVector<QVector<double>> vec;

    // 切割成每一行
    QStringList qsl = str.replace("\n", "").trimmed().split(QRegExp("\\s*;"));

    // 第一行
    QVector<double> vecLine = getFilterDataOfEveryLine(qsl[0], colNum);
    vec.push_back(vecLine);

    // 从第二行开始
    for(int i = 1; i < qsl.length() && qsl[i] != ""; i++){

        int curColNum = 0;
        QVector<double> vecLine = getFilterDataOfEveryLine(qsl[i], curColNum);
        if(curColNum == colNum){
            vec.push_back(vecLine);
        }
    }
    return vec;
}

QVector<double> MainWindow::getFilterDataOfEveryLine(QString &str, int & colNum){

    QVector<double> vec;

    // 切割成每一个元素
    QStringList qsl = str.replace("\n","").trimmed().split(QRegExp("\\s*,"));

    // 本行元素个数
    colNum = qsl.length();
    for(int i = 0; i < qsl.length() && qsl[i] != ""; i++){

        double d = (double)qsl.at(i).toDouble();

        vec.push_back(d);
    }
    return vec;
}
/**
 * 灰度直方图均衡
 * @brief MainWindow::on_actionhisteq_triggered
 */
void MainWindow::on_actionhisteq_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        showImage(ImageProcessing::histEquilibrium(img));
    }
}

/**
 * RGB直方图均衡
 * @brief MainWindow::on_actionhist_eq_for_rbg_triggered
 */
void MainWindow::on_actionhist_eq_for_rbg_triggered()
{
    QImage img;

    if(getDisplayImage(img)){

        showImage(ImageProcessing::histEquilibriumForRgb(img));
//        showImage(ImageProcessing::histEquilibriumForRgbNoUse(img));
    }
}

/**
 * HSI直方图均衡
 * @brief MainWindow::on_actionhist_eq_for_hsi_triggered
 */
void MainWindow::on_actionhist_eq_for_hsi_triggered()
{
    QImage img;

//    ImageProcessing::test();
    if(getDisplayImage(img)){

        showImage(ImageProcessing::histEquilibriumByHSI(img));
    }
}
