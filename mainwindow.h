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
  MainWindow(Utilities& util, Database &datab, Player &p, Filescan &fsinit, QApplication& aapp, QWidget *parent = 0);
  void setPath(char* path);
  QMenuBar* createMenuBar();
public slots:
  void menuScan();
  void menuAddFile();
  void menuSettings();
  void menuMaximise();
  void menuMinimise();
  void menuMinimiseToTray();
  void menuDocumentation();
  void menuAbout();
  void menuShowFileProviders();
  void menuShowOfflineFiles();
  void trayIconClicked(QSystemTrayIcon::ActivationReason);

signals:
  void filescanRequest();

private:
  //layout of the main window
  void makeTrayIcon();
  QWidget* centralwidget;
  QMenuBar* menubar;
  QGridLayout* mainlayout;
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
  QApplication& app;
  QSystemTrayIcon* trayicon;
};

#endif // MAINWINDOW_H
