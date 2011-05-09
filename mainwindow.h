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

//The program's main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Utilities& util, Database &datab, Player &p, Filescan &fsinit, QMenuBar* menu, QApplication& aapp, QWidget *parent = 0);
    void setPath(char* path);
    //fill menubar with menus
    QMenuBar* createMenuBar();
public slots:
    //open settings panel
    void menuSettings();
    //maximise and minimise
    void menuMaximise();
    void menuMinimise();
    void menuMinimiseToTray();
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    //make new playlists
    void menuNewPlaylist();
    void menuNewSmartPlaylist();

signals:
    //request a new filescan
    void filescanRequest();
    void repeatsig(bool, bool);

private slots:
    //increase volume
    void menuVolUp();
    //decrease volume
    void menuVolDown();
    //mute volume
    void menuMute();
    //toggle repeat
    void menuRepeat();//was menuShuffle, but is actually repeat

private:
    void makeTrayIcon();
    //the widget that is displayed in window
    QWidget* centralwidget;
    QMenuBar* menubar;
    //puts parts of window in right place
    QGridLayout* mainlayout;
    QAction* mutemenu;
    void initialiseGrid();
    //called when window resized
    void resizeEvent(QResizeEvent *);
    //called when window moved
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
    int volmem;
    QApplication& app;
    QSystemTrayIcon* trayicon;
};

#endif // MAINWINDOW_H
