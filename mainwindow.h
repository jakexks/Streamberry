#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "sidebarcontroller.h"
#include "topbarcontroller.h"
#include "librarycontroller.h"
#include "playbackcontroller.h"
#include "utilities.h"
#include "database.h"
#include "player.h"
#include "filescan.h"
#include <QSystemTrayIcon>


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Utilities& util, Database &datab, Player &p, Filescan &fsinit, QMenuBar* menu, QApplication& aapp, QWidget *parent = 0);
  void setPath(char* path);
  QMenuBar* createMenuBar();
public slots:
  void menuSettings();
  void menuMaximise();
  void menuMinimise();
  void menuMinimiseToTray();
  void trayIconClicked(QSystemTrayIcon::ActivationReason);
  void menuNewPlaylist();
  void menuNewSmartPlaylist();
//  Code in the header is bad.
//  void closeEvent(QCloseEvent *event)
//  {
//      QSettings settings;
//      settings.setValue("pos", pos());
//      settings.setValue("normalgeo", normalGeometry());
//      settings.setValue("ismax", isMaximized());
//  }

signals:
  void filescanRequest();
  void repeatsig(bool, bool);

private slots:
  void menuVolUp();
  void menuVolDown();
  void menuMute();
  void menuRepeat();

private:
  //layout of the main window
  void makeTrayIcon();
  QWidget* centralwidget;
  QMenuBar* menubar;
  QGridLayout* mainlayout;
  QAction* mutemenu;
  void initialiseGrid();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);
  //void changeEvent(QEvent *event );

  //widgets for each section of screen
  SidebarController* sidebarcontroller;
  TopbarController* topbarcontroller;
  LibraryController* librarycontroller;
  PlaybackController* playbackcontroller;

  Utilities& util;
  Database& db;
  Player& player;
  Filescan& fs;
  int volmem;
  QApplication& app;
  QSystemTrayIcon* trayicon;
};

#endif // MAINWINDOW_H
