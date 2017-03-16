#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ltDlg(new LogTransformationDlg)
{
    ui->setupUi(this);

    gpi = new MyMouseEvent();
    gs = new QGraphicsScene(this);

    fileDialog = new QFileDialog(this);

    connect(ltDlg,SIGNAL(confirmed(double)),this,SLOT(on_confirmed_accepted(double)));
}

MainWindow::~MainWindow(){

    delete gpi;
    delete gs;
    delete ui;

    delete fileDialog;
}

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

void MainWindow::on_btn_last_clicked()
{
    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + --curIndex) % images.length()].absoluteFilePath()));
}

void MainWindow::on_btn_next_clicked()
{

    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + ++curIndex) % images.length()].absoluteFilePath()));
}

void MainWindow::on_btn_reset_clicked()
{
    reSet();
}

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

void MainWindow::on_actionrgb2gray_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::rbg2gray(img);
        showImage(img);
    }
}

void MainWindow::on_actionpixelReverse_triggered()
{
    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::pixelReverse(img);
        showImage(img);
    }
}

void MainWindow::on_actionlog_trans_triggered()
{
    ltDlg->exec();
}

void MainWindow::on_confirmed_accepted(double c)
{
    // qDebug() << "accepted number is:" <<c << endl;

    QImage img;

    if(getDisplayImage(img)){
        img = ImageProcessing::logTransformation(img, c);
        showImage(img);
    }
}
