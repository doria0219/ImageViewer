#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileDialog = new QFileDialog(this);

    gs = new QGraphicsScene(this);
    gpi = new MyMouseEvent();
}

MainWindow::~MainWindow(){

    delete gpi;
    delete gs;
    delete ui;

    delete fileDialog;
}

void MainWindow::on_btn_open_clicked(){

    QStringList files;
    if(fileDialog->exec()){
        files = fileDialog->selectedFiles();
    }

    // 如果没有选中文件
    if(files.length() == 0){
        return;
    }
    img = QImage(files[0]);
    showImage(img);
}

void MainWindow::showImage(QImage img){

    gpi->setPixmap(QPixmap::fromImage(img));

    gs->addItem(gpi);

    ui->graphicsView->setScene(gs);
}

void MainWindow::on_btn_last_clicked()
{

}

void MainWindow::on_btn_next_clicked()
{

}
