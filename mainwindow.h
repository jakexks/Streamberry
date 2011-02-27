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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Utilities& util, Database &datab, Player &p, QWidget *parent = 0);
    void setPath(char* path);
    QMenuBar* createMenuBar();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void giveNewLibrary(QList<QString>* sortcols, QList<QString>* order);
signals:
    void quitSignal();
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
};

#endif // MAINWINDOW_H
