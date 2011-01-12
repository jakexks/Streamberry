#include "mainwindow.h"
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(500, 500);
    centralWidget = new QWidget();
    mainLayout = new QGridLayout(centralWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->setRowMinimumHeight(0, 70);
    mainLayout->setRowStretch(0, 0);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowMinimumHeight(2, 50);
    mainLayout->setRowStretch(2, 0);
    mainLayout->setColumnMinimumWidth(0, 190);
    mainLayout->setColumnStretch(0, 0);
    mainLayout->setColumnStretch(1, 1);

    topbar = makeTopBar();
    leftbar = makeLeftBar();
    bottombar = makeBottomBar();
    rightside = makeRightSide();

    mainLayout->addWidget(topbar, 0, 0, 1, 2);
    mainLayout->addWidget(leftbar, 1, 0);
    mainLayout->addWidget(rightside, 1, 1);
    mainLayout->addWidget(bottombar, 2, 0, 1, 2);

    setCentralWidget(centralWidget);
}

QWidget* MainWindow::makeTopBar() {
    QWidget *tempw = new QWidget();
    tempw->setObjectName("topBarWidget");
    tempw->setStyleSheet("QWidget#topBarWidget { background-color: #DDDDDD }");

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
    tempw->setStyleSheet("QWidget#bottomBarWidget { background-color: #DDDDDD }");

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
