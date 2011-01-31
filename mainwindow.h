#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "playlists.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(char* path, QWidget *parent = 0);
    ~MainWindow();
    void setPath(char* path);
    QMenuBar* createMenuBar();

private:
    QWidget* centralWidget;
    QGridLayout* mainLayout;
    //properties and method for each section of screen
    QWidget* topbar;
    QWidget* makeTopBar();
    Playlists* sidebar;
    QWidget* leftbar;
    QWidget* makeLeftBar();
    QWidget* bottombar;
    QWidget* makeBottomBar();
    QWidget* rightside;
    QWidget* makeRightSide();
    //path of the executable
    QString expath;
    QMenuBar* menubar;
};

#endif // MAINWINDOW_H
