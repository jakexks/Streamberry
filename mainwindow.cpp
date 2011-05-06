#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include <QGridLayout>
#include "sidebarcontroller.h"
#include "albumpanel.h"
#include "librarycontroller.h"
#include "utilities.h"
#include "database.h"
#include "filescan.h"
#include <QSystemTrayIcon>
#include "settingsdialog.h"
#include <plsmartnew.h>
#include <plnormalnew.h>
#include <previewpane.h>

#define TOPBARHEIGHT 26
#define BOTTOMBARHEIGHT 90

MainWindow::MainWindow(Utilities& utilities, Database &datab, Player &p, Filescan &fsinit, QMenuBar* menu, QApplication& aapp, QWidget *parent)
  : QMainWindow(parent), util(utilities), db(datab), player(p), fs(fsinit), app(aapp)
{
  //set window properties
  //menubar created in main to fix bug on Mac
  menubar = menu;


  setStyleSheet(util.getStylesheet());

  //    QSettings settings;
  //    QPoint pos = settings.value("pos").toPoint();
  //    QSize size = settings.value("normalgeo", QRect(pos, QSize(900,625))).toRect().size();
  //    resize(size);
  //    move(pos);

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

  if((temp = db.getSetting("isMaximised")) != NULL)
  {
    if(temp=="1")
      setWindowState(Qt::WindowMaximized);
    else
      setWindowState(Qt::WindowNoState);
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
  librarycontroller = new LibraryController(util, db, player, topbarcontroller->getSearchbar());
  sidebarcontroller = new SidebarController(util, db, librarycontroller);
  playbackcontroller = new PlaybackController(util, player);
  mainlayout->addWidget(topbarcontroller->getWidget(), 0, 1, 1, 1);
  mainlayout->addWidget(sidebarcontroller->getWidget(), 0, 0, 2, 1);
  mainlayout->addWidget(librarycontroller->getWidget(), 1, 1);
  mainlayout->addWidget(playbackcontroller->getWidget(), 2, 0, 1, 2);

  QObject::connect(playbackcontroller, SIGNAL(nextFile()), librarycontroller, SLOT(playNextFile()));
  QObject::connect(playbackcontroller, SIGNAL(prevFile()), librarycontroller, SLOT(playPrevFile()));
  QObject::connect(topbarcontroller, SIGNAL(musicVideoCheckStateChanged(int)), librarycontroller, SLOT(musicVideoFilter(int)));
  QObject::connect(librarycontroller, SIGNAL(songInfoData(QString,QString,QString,QString)), playbackcontroller->getSongInfo(), SLOT(updateLabels(QString,QString,QString,QString)));
  QObject::connect(&fs, SIGNAL(finishedFileScan()), librarycontroller, SLOT(resetQueue()));
  QObject::connect(this, SIGNAL(filescanRequest()), &fs, SLOT(scan()));
  QObject::connect(topbarcontroller, SIGNAL(goBackPlease()), librarycontroller, SLOT(goBack()));
  QObject::connect(topbarcontroller, SIGNAL(goForwardPlease()), librarycontroller, SLOT(goForward()));
  QObject::connect(librarycontroller, SIGNAL(setSearchBoxText(QString)), topbarcontroller, SLOT(setSearchText(QString)));
  QObject::connect(librarycontroller, SIGNAL(setSelectedPlaylist(QString)), sidebarcontroller, SLOT(setSelectedPlaylist(QString)));
  QObject::connect(playbackcontroller, SIGNAL(sPress()), librarycontroller, SLOT(shuffleSlot()));
  QObject::connect(playbackcontroller, SIGNAL(rPress(bool, bool)), librarycontroller, SLOT(repeatSlot(bool, bool)));
  QObject::connect(librarycontroller, SIGNAL(selectVideo()), sidebarcontroller, SLOT(setVideoSelected()));

  PreviewPane* preview = sidebarcontroller->getPreviewPane();
  QObject::connect(&player, SIGNAL(playingalbumart()), preview, SLOT(displayAlbumArt()));  ///CHANGE THIS ONCE ALBUMART WORKS
  QObject::connect(&player, SIGNAL(settracklength(int)), preview, SLOT(settracklength(int)));
  QObject::connect(&player, SIGNAL(settrackprogress(float)), preview, SLOT(settrackprogress(float)));
  QObject::connect(&player, SIGNAL(setAlbumArtDefault()), preview, SLOT(setDefault()));

  this->setWindowTitle("Streamberry");
  QString iconpath = util.getExecutePath();
  iconpath += "images/icon.ico";
  this->setWindowIcon((QIcon(iconpath)));
  makeTrayIcon();

  createMenuBar();
  this->setMenuBar(menubar);
  setCentralWidget(centralwidget);

}

void MainWindow::makeTrayIcon()
{
  QString iconpath = util.getExecutePath();
  iconpath += "images/icon.ico";
  trayicon = new QSystemTrayIcon(QIcon(iconpath));
  QObject::connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
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
  menubar->setNativeMenuBar(true);
  QMenu* menus[6];
  QAction* actions[23];
  menus[0] = menubar->addMenu("File");
  menus[1] = menubar->addMenu("Edit");
  menus[2] = menubar->addMenu("Controls");
  menus[4] = menubar->addMenu("Window");

#ifdef Q_WS_MAC
  menubar->addMenu("Help");
#endif

  //FILE MENU/////////////////////////////////////////////////////////////////
  actions[0] = menus[0]->addAction("Exit");
  QObject::connect(actions[0], SIGNAL(triggered()), &app, SLOT(quit()) );

  //EDIT MENU//
  actions[21] = menus[1]->addAction("New Filter Playlist");
  QObject::connect(actions[21], SIGNAL(triggered()), this, SLOT(menuNewPlaylist()));
  actions[22] = menus[1]->addAction("New Playlist");
  QObject::connect(actions[22], SIGNAL(triggered()), this, SLOT(menuNewSmartPlaylist())); //THIS LOOKS WRONG BUT IT'S RIGHT
  menus[1]->addSeparator();
#ifdef Q_WS_WIN
  actions[5] = menus[1]->addAction("Settings");
#endif

#ifdef Q_WS_MAC
  actions[5] = menus[1]->addAction("Preferences");
#endif

#ifdef Q_WS_X11
  actions[5] = menus[1]->addAction("Preferences");
#endif

  QObject::connect(actions[5], SIGNAL(triggered()), this, SLOT(menuSettings()));

  //CONTROLS MENU///////////////////////////////////////////////////////////////
  actions[6] = menus[2]->addAction("Play/Pause");
  QObject::connect(actions[6], SIGNAL(triggered()), &player, SLOT(playControl()));
  actions[6]->setShortcut(QKeySequence("Ctrl+P"));
  menus[2]->addSeparator();

  actions[8] = menus[2]->addAction("Next Track");
  QObject::connect(actions[8], SIGNAL(triggered()), librarycontroller, SLOT(playNextFile()));
  actions[8]->setShortcut(Qt::Key_Right);

  actions[9] = menus[2]->addAction("Previous Track");
  QObject::connect(actions[9], SIGNAL(triggered()), librarycontroller, SLOT(playPrevFile()));
  //QObject::connect(actions[9], SIGNAL(triggered()), playbackcontroller, SIGNAL(prevFile()));

  actions[9]->setShortcut(Qt::Key_Left);
  menus[2]->addSeparator();

  actions[11] = menus[2]->addAction("Volume Up");
  actions[11]->setShortcut(QKeySequence(">"));
  QObject::connect(actions[11], SIGNAL(triggered()), this, SLOT(menuVolUp()));

  actions[12] = menus[2]->addAction("Volume Down");
  actions[12]->setShortcut(QKeySequence("<"));
  QObject::connect(actions[12], SIGNAL(triggered()), this, SLOT(menuVolDown()));

  actions[13] = menus[2]->addAction("Mute");
  actions[13]->setCheckable(true);
  QObject::connect(actions[13], SIGNAL(triggered()), this, SLOT(menuMute()));
  actions[13]->setShortcut(QKeySequence("Ctrl+M"));
  mutemenu = actions[13];
  menus[2]->addSeparator();

  actions[14] = menus[2]->addAction("Repeat All");
  actions[14]->setCheckable(true);
  QObject::connect(actions[14], SIGNAL(triggered()), this, SLOT(menuShuffle()));
  QObject::connect(this, SIGNAL(repeatsig(bool, bool)), librarycontroller, SLOT(repeatSlot(bool, bool)));

  actions[15] = menus[2]->addAction("Shuffle");
  actions[15]->setCheckable(true);
  QObject::connect(actions[15], SIGNAL(triggered()), playbackcontroller, SLOT(shufflePress()));

  //WINDOW MENU//////////////////////////////////////////////////////////////////
#if defined(Q_OS_MAC)
  actions[19] = menus[4]->addAction("Minimize");
  actions[19]->setShortcut(QKeySequence("Ctrl+M"));
  QObject::connect(actions[19], SIGNAL(triggered()), this, SLOT(menuMinimise()));
#endif

#if defined(Q_OS_MAC)
      actions[18] = menus[4]->addAction("Zoom");
#else
  actions[18] = menus[4]->addAction("Maximise");
  QObject::connect(actions[18], SIGNAL(triggered()), this, SLOT(menuMaximise()));

  actions[19] = menus[4]->addAction("Minimise");
  QObject::connect(actions[19], SIGNAL(triggered()), this, SLOT(menuMinimise()));

  actions[20] = menus[4]->addAction("Minimise to Tray");
  QObject::connect(actions[20], SIGNAL(triggered()), this, SLOT(menuMinimiseToTray()));
#endif

  return menubar;
}

void MainWindow::resizeEvent(QResizeEvent *resize)
{
  QSize size = resize->size();

  if(isMaximized())
    db.storeSetting("isMaximised", "1");
  else
  {
    QString winsize = QString::number(size.width());
    winsize += "|";
    winsize += QString::number(size.height());
    db.storeSetting("windowSize", winsize);
    db.storeSetting("isMaximised", "0");
  }

  player.resizeVideo();
  sidebarcontroller->updateplaylistbar( (int)(size.height()/89.25) );
}

void MainWindow::moveEvent(QMoveEvent *move)
{
  QPoint pos = move->pos();
  QString winpos = QString::number(pos.x());
  winpos += "|";
  winpos += QString::number(pos.y());
  db.storeSetting("windowPos", winpos);
  db.storeSetting("isMaximised", "0");
}

void MainWindow::menuSettings()
{
  SettingsDialog settingsdialog(db, fs, this);
  settingsdialog.setModal(true);
  settingsdialog.show();
  settingsdialog.exec();
}

void MainWindow::menuMaximise()
{
  this->setWindowState(Qt::WindowMaximized);
}

void MainWindow::menuMinimise()
{
  this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::menuMinimiseToTray()
{
  trayicon->setVisible(true);
  this->hide();
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason)
{
  this->show();
}

void MainWindow::menuNewPlaylist()
{
  PLSmartNew(&util, db);

  QString size = db.getSetting("windowSize");
  QStringList list = size.split('|');
  int size2 = 0;
  if(list.size()==2)
    size2 = list.at(1).toInt();
  sidebarcontroller->updateplaylistbar( size2 );
}


void MainWindow::menuNewSmartPlaylist()
{
  PLNormalNew(&util, db);

  QString size = db.getSetting("windowSize");
  QStringList list = size.split('|');
  int size2 = 0;
  if(list.size()==2)
    size2 = list.at(1).toInt();
  sidebarcontroller->updateplaylistbar( size2 );
}

void MainWindow::menuVolUp()
{
  int i = playbackcontroller->getvolpos();
  i = i + 20;
  if(i>100)
    i=100;
  playbackcontroller->setvolumepos(i);
  player.changeVolume(i);
}

void MainWindow::menuVolDown()
{
  int i = playbackcontroller->getvolpos();
  i = i - 20;
  if(i<0)
    i=0;
  playbackcontroller->setvolumepos(i);
  player.changeVolume(i);
}

void MainWindow::menuMute()
{
  if(!mutemenu->isChecked())
  {
    playbackcontroller->setvolumepos(volmem);
    player.changeVolume(volmem);
  }
  else
  {
    volmem = playbackcontroller->getvolpos();
    playbackcontroller->setvolumepos(0);
    player.changeVolume(0);
  }
}

void MainWindow::menuShuffle()
{
  emit repeatsig(false, true);
}

/*void MainWindow::changeEvent ( QEvent *event )
{
  if( event->type() == QEvent::WindowStateChange )
  {
    if( isMinimized() )
    {
      trayicon->setVisible(true);
      this->setVisible(false);
      this->hide();
    }
  }
}*/

