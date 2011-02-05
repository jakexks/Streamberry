#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QGridLayout>
#include "sidebarcontroller.h"
#include "albumpanel.h"
#include "librarycontroller.h"
#include "utilities.h"
#include "database.h"

#define TOPBARHEIGHT 36
#define BOTTOMBARHEIGHT 90

MainWindow::MainWindow(Utilities& utilities, Database &datab, QWidget *parent)
    : QMainWindow(parent), util(utilities), db(datab)
{
    //set window properties
    menubar = createMenuBar();
    this->setWindowTitle("Streamberry");
    this->setMenuBar(menubar);
    resize(850, 600);

    //initialise window layout
    centralwidget = new QWidget();
    mainlayout = new QGridLayout(centralwidget);
    initialiseGrid();

    //initialise controllers and add widgets to window
    sidebarcontroller = new SidebarController(util);
    librarycontroller = new LibraryController(util);
    playbackcontroller = new PlaybackController(util);

    mainlayout->addWidget(sidebarcontroller->getWidget(), 0, 0, 2, 1);
    mainlayout->addWidget(librarycontroller->getWidget(), 1, 1);
    mainlayout->addWidget(playbackcontroller->getWidget(), 2, 0, 1, 2);

    QList<QString> fields;
    QList<QString> order;
    fields.append("Album");
    order.append("DESC");

    QList<QSqlRecord> *result = db.searchDb(0, "", fields, order);
    librarycontroller->fillData(result);

    QObject::connect(librarycontroller, SIGNAL(needNewLibrary(QList<QString>*,QList<QString>*)), this, SLOT(giveNewLibrary(QList<QString>*,QList<QString>*)));

    setCentralWidget(centralwidget);
}

void MainWindow::initialiseGrid()
{
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);
    mainlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
    mainlayout->setRowStretch(0, 0);
    mainlayout->setRowStretch(1, 1);
    mainlayout->setRowMinimumHeight(2, BOTTOMBARHEIGHT);
    mainlayout->setRowStretch(2, 0);
    mainlayout->setColumnMinimumWidth(0, 190);
    mainlayout->setColumnStretch(0, 0);
    mainlayout->setColumnStretch(1, 1);
}

QMenuBar* MainWindow::createMenuBar()
{
    QMenuBar* menu = new QMenuBar();
    menu->setNativeMenuBar(true);
//    QMenu* file = menu->addMenu("File");
//    QMenu* view = menu->addMenu("View");
//    QMenu* controls = menu->addMenu("View");
//    QMenu* window = menu->addMenu("Window");
//    QMenu* help = menu->addMenu("Help");
    menu->addMenu("File");
    menu->addMenu("View");
    menu->addMenu("View");
    menu->addMenu("Window");
    menu->addMenu("Help");
    return menu;
}

void MainWindow::giveNewLibrary(QList<QString> *sortcols, QList<QString> *order)
{
    QList<QSqlRecord> *result = db.searchDb(0, "", *sortcols, *order);
    librarycontroller->fillData(result);
    delete sortcols;
    delete order;
}
