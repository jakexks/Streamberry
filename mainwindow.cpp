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

    QString temp;
    if((temp = db.getSetting("windowSize")) != NULL)
    {

        QStringList list = temp.split('|');
        if(list.size()==2)
        {
            resize(list.at(0).toInt(), list.at(1).toInt());
        }
        else
        {
            resize(900, 625);
        }
    }
    else
    {
        resize(900, 625);
    }

    if((temp = db.getSetting("windowPos")) != NULL)
    {
        QStringList list = temp.split('|');
        if(list.size()==2)
        {
            move(list.at(0).toInt(), list.at(1).toInt());
        }
    }

    //initialise window layout
    centralwidget = new QWidget();
    mainlayout = new QGridLayout(centralwidget);
    initialiseGrid();

    //initialise controllers and add widgets to window
    sidebarcontroller = new SidebarController(util);
    librarycontroller = new LibraryController(util, db);
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

void MainWindow::resizeEvent(QResizeEvent *resize)
{
    QSize size = resize->size();
    QString winsize = QString::number(size.width());
    winsize += "|";
    winsize += QString::number(size.height());
    db.storeSetting("windowSize", winsize);
}

void MainWindow::moveEvent(QMoveEvent *move)
{
    QPoint pos = move->pos();
    QString winpos = QString::number(pos.x());
    winpos += "|";
    winpos += QString::number(pos.y());
    db.storeSetting("windowPos", winpos);
}