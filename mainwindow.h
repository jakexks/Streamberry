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


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Utilities& util, Database &datab, Player &p, Filescan &fsinit, QWidget *parent = 0);
  void setPath(char* path);
  QMenuBar* createMenuBar();
public slots:
  void giveNewLibrary(QList<QString>* sortcols, QList<QString>* order);
  void menuExitStreamberry();
  void menuScan();
  void menuCleanScan();
  void menuAddFile();
  void menuOptions();
  void menuMaximise();
  void menuMinimise();
  void menuMinimiseToTray();
  void menuDocumentation();
  void menuAbout();
  void menuShowFileProviders();
  void menuShowOfflineFiles();

private:
  //layout of the main window
  QWidget* centralwidget;
  QMenuBar* menubar;
  QGridLayout* mainlayout;
  void initialiseGrid();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

  //widgets for each section of screen
  SidebarController* sidebarcontroller;
  TopbarController* topbarcontroller;
  LibraryController* librarycontroller;
  PlaybackController* playbackcontroller;

  Utilities& util;
  Database& db;
  Player& player;
  Filescan& fs;
};

#endif // MAINWINDOW_H
