#include "mainwindow.h"
#include <QtGui>
#include <QDebug>

#define TOPBARHEIGHT 34
#define BOTTOMBARHEIGHT 83

void MainWindow::setPath(char* path)
{
    expath = path;
}

MainWindow::MainWindow(char* path, QWidget *parent)
    : QMainWindow(parent)
{
    expath = QString(path);

    #ifndef Q_OS_WIN
        expath.resize(expath.lastIndexOf('/'));
    #endif
    #ifdef Q_OS_WIN
        expath.resize(expath.lastIndexOf('\'));
    #endif

    expath += "/";

    this->setWindowTitle("Streamberry");
    resize(500, 500);
    centralWidget = new QWidget();
    mainLayout = new QGridLayout(centralWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->setRowMinimumHeight(0, TOPBARHEIGHT);
    mainLayout->setRowStretch(0, 0);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
    mainLayout->setRowStretch(2, 0);
    mainLayout->setColumnMinimumWidth(0, 190);
    mainLayout->setColumnStretch(0, 0);
    mainLayout->setColumnStretch(1, 1);

    topbar = makeTopBar();
    leftbar = makeLeftBar();
    bottombar = makeBottomBar();
    rightside = makeRightSide();

    mainLayout->addWidget(topbar, 0, 1);
    mainLayout->addWidget(leftbar, 0, 0, 2, 1);
    mainLayout->addWidget(rightside, 1, 1);
    mainLayout->addWidget(bottombar, 2, 0, 1, 2);

    setCentralWidget(centralWidget);
}

QWidget* MainWindow::makeTopBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("topBarWidget");
    QString style = "QWidget#topBarWidget { background-image: url(";
    style += expath;
    style += "images/bgTopBar.png); background-repeat: repeat-x }";
    tempw->setStyleSheet(style);

    QGridLayout *temp = new QGridLayout(tempw);
    return tempw;
}

QWidget* MainWindow::makeLeftBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("leftBarWidget");
    tempw->setStyleSheet("QWidget#leftBarWidget { background-color: #D9DFE7 }");

    QGridLayout *temp = new QGridLayout(tempw);
    //put widgets into layout here
    return tempw;
}

QWidget* MainWindow::makeBottomBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("bottomBarWidget");
    QString style = "QWidget#bottomBarWidget { background-image: url(";
    style += expath;
    style += "images/bgBottomBar.png); background-repeat: repeat-x }";
    tempw->setStyleSheet(style);

    QGridLayout *temp = new QGridLayout(tempw);
    //put widgets into layout here
    return tempw;
}

QWidget* MainWindow::makeRightSide() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("rightSideWidget");
    tempw->setStyleSheet("QWidget#rightSideWidget { background-color: #FFFFFF }");

    QGridLayout *temp = new QGridLayout(tempw);
    temp->setMargin(0);

    QTableWidget *tableWidget = new QTableWidget(12, 3);
    tableWidget->setObjectName("libraryTableWidget");
    tableWidget->setStyleSheet("QTableWidget#libraryTableWidget { border:none; }");
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(5);

    QTableWidgetItem *newItem = new QTableWidgetItem("Hello");
    tableWidget->setItem(5, 2, newItem);
    temp->addWidget(tableWidget);

    return tempw;
}

MainWindow::~MainWindow()
{
}
