#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "sidebarcontroller.h"
#include "topbarcontroller.h"
#include "librarycontroller.h"
#include "playbackcontroller.h"
#include "utilities.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Utilities& util, QWidget *parent = 0);
    void setPath(char* path);
    QMenuBar* createMenuBar();

private:
    //layout of the main window
    QWidget* centralwidget;
    QMenuBar* menubar;
    QGridLayout* mainlayout;
    void initialiseGrid();

    //widgets for each section of screen
    SidebarController* sidebarcontroller;
    TopbarController* topbarcontroller;
    LibraryController* librarycontroller;
    PlaybackController* playbackcontroller;

    Utilities& util;
};

#endif // MAINWINDOW_H
