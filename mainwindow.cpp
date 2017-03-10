#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gpi = new MyMouseEvent();
    gs = new QGraphicsScene(this);

    fileDialog = new QFileDialog(this);
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

    gpi->setPixmap(QPixmap::fromImage(img));

    gs->addItem(gpi);

    ui->graphicsView->setScene(gs);

    reSet();
}

void MainWindow::on_btn_last_clicked()
{
    // 移除当前对象
    gs->removeItem(gpi);
    gpi = new MyMouseEvent();
    if(images.length() == 0){
        return;
    }

    showImage(QImage(images[(images.length() + --curIndex) % images.length()].absoluteFilePath()));
}

void MainWindow::on_btn_next_clicked()
{
    gs->removeItem(gpi);
    gpi = new MyMouseEvent();
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
