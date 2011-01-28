#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include "sbsearchbar.h"

#define TOPBARHEIGHT 33
#define BOTTOMBARHEIGHT 83

MainWindow::MainWindow(char* path, QWidget *parent)
    : QMainWindow(parent)
{
    expath = QString(path);
    expath.resize(expath.lastIndexOf('/'));
    expath += "/";
    menubar = createMenuBar();

    this->setWindowTitle("Streamberry");
    this->setMenuBar(menubar);
    resize(850, 600);
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

    //Grids to hold objects in
    QGridLayout *outer = new QGridLayout(tempw);
    outer->setMargin(0);
    outer->setSpacing(0);
    QGridLayout *innerleft = new QGridLayout();
    innerleft->setMargin(0);
    innerleft->setSpacing(0);
    QGridLayout *innermiddle = new QGridLayout();
    innermiddle->setMargin(0);
    innermiddle->setSpacing(0);
    QGridLayout *innerright = new QGridLayout();
    innerright->setMargin(0);
    innerright->setSpacing(0);
    //Buttons for menu
    QPushButton *back = new QPushButton();
    QPushButton *forward = new QPushButton();
    QPushButton *music = new QPushButton();
    QPushButton *videos = new QPushButton();
    SBSearchBar *search = new SBSearchBar(expath);
    //Split it into three sections
    outer->addLayout(innerleft, 1, 0);
    outer->addLayout(innermiddle, 1, 1);
    outer->addLayout(innerright, 1, 2);
    //set which rows and columns in outer layout are stretchy
    outer->setRowStretch(0, 1);
    outer->setRowStretch(1, 0);
    outer->setRowStretch(2, 1);
    outer->setColumnStretch(0, 0);
    outer->setColumnStretch(1, 1);
    outer->setColumnStretch(2, 0);
    //Set which are stretchy in inner layouts
    innermiddle->setColumnStretch(0, 1);
    innermiddle->setColumnStretch(1, 0);
    innermiddle->setColumnStretch(2, 0);
    innermiddle->setColumnStretch(3, 1);
    //Set measurements of spacer columns
    innerleft->setColumnMinimumWidth(0, 15);
    innerleft->setColumnMinimumWidth(2, 10);
    innerright->setColumnMinimumWidth(1, 35);
    //Add buttons to each section
    innerleft->addWidget(back, 0, 1);
    innerleft->addWidget(forward, 0, 3);
    innermiddle->addWidget(music, 0, 1);
    innermiddle->addWidget(videos, 0, 2);
    innerright->addWidget(search, 0, 0);
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

    QTableWidgetItem *newItem = new QTableWidgetItem(expath);
    tableWidget->setItem(5, 2, newItem);
    temp->addWidget(tableWidget);

    return tempw;
}

QMenuBar* MainWindow::createMenuBar()
{
    QMenuBar* menu = new QMenuBar();
    menu->setNativeMenuBar(true);
    QMenu* file = menu->addMenu("File");
    QMenu* view = menu->addMenu("View");
    QMenu* controls = menu->addMenu("View");
    QMenu* window = menu->addMenu("Window");
    QMenu* Help = menu->addMenu("Help");
    return menu;
}

MainWindow::~MainWindow()
{
}
