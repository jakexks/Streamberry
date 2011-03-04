#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QGridLayout>
#include "sidebarcontroller.h"
#include "albumpanel.h"
#include "librarycontroller.h"
#include "utilities.h"
#include "database.h"

#define TOPBARHEIGHT 26
#define BOTTOMBARHEIGHT 90

MainWindow::MainWindow(Utilities& utilities, Database &datab, Player &p, QWidget *parent)
  : QMainWindow(parent), util(utilities), db(datab), player(p)
{
    //set window properties
    menubar = createMenuBar();
    this->setWindowTitle("Streamberry");
    this->setMenuBar(menubar);
    setStyleSheet(util.getStylesheet());


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
    centralwidget->setObjectName("centralWidget");
    centralwidget->setStyleSheet(util.getStylesheet());
    mainlayout = new QGridLayout(centralwidget);
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);
    initialiseGrid();

    //initialise controllers and add widgets to window
    topbarcontroller = new TopbarController(util);
    sidebarcontroller = new SidebarController(util);
    librarycontroller = new LibraryController(util, db, player);
    playbackcontroller = new PlaybackController(util, player);

    mainlayout->addWidget(topbarcontroller->getWidget(), 0, 1, 1, 1);
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
  QObject::connect(playbackcontroller, SIGNAL(nextFile()), librarycontroller, SLOT(playNextFile()));
  QObject::connect(playbackcontroller, SIGNAL(prevFile()), librarycontroller, SLOT(playPrevFile()));

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

// Defines all the actions available in the menus
//void MainWindow::createActions()
//{
//    openaction = new QAction(tr("&Open"), this);
//    openaction->setShortcut(tr("Ctrl+O"));
//    openaction->setStatusTip(tr("Open a media file stored on your PC"));
//connect(openaction, SIGNAL(), this, SLOT(openFile()));
//}

QMenuBar* MainWindow::createMenuBar()
{
  QMenuBar* menubar = new QMenuBar();
  menubar->setNativeMenuBar(true);
  QMenu* menus[6];
  menus[0] = menubar->addMenu("File");
  menus[1] = menubar->addMenu("Edit");
  menus[2] = menubar->addMenu("Controls");
  menus[3] = menubar->addMenu("View");
  menus[4] = menubar->addMenu("Window");
  menus[5] = menubar->addMenu("Help");

  menus[0]->addAction("Exit Streamberry");

  menus[1]->addAction("Scan Folders for Media");
  menus[1]->addAction("Fresh Scan for Media");
  menus[1]->addAction("Add Individual File");
  menus[2]->addSeparator();
  menus[1]->addAction("Options");

  menus[2]->addAction("Play/Pause");
  menus[2]->addSeparator();
  menus[2]->addAction("Next Track");
  menus[2]->addAction("Previous Track");
  menus[2]->addSeparator();
  menus[2]->addAction("Volume Up");
  menus[2]->addAction("Volume Down");
  menus[2]->addSeparator();
  menus[2]->addAction("Toggle Repeat");
  menus[2]->addAction("Toggle Shuffle");

  menus[3]->addAction("Show File Providers");
  menus[3]->addAction("Show Offline Files");

  menus[4]->addAction("Maximise");
  menus[4]->addAction("Minimise");
  menus[4]->addAction("Minimise to Tray");

  menus[5]->addAction("Documentation");
  menus[5]->addAction("About Streamberry");
  return menubar;
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

  sidebarcontroller->updateplaylistbar( (int)(size.height()/89.25) );
}

void MainWindow::moveEvent(QMoveEvent *move)
{
  QPoint pos = move->pos();
  QString winpos = QString::number(pos.x());
  winpos += "|";
  winpos += QString::number(pos.y());
  db.storeSetting("windowPos", winpos);
}
