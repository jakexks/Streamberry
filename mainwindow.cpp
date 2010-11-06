#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new QGraphicsView;
    setCentralWidget(canvas);
    this->setStatusBar(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}
