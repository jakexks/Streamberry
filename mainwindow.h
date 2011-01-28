#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(char* path, QWidget *parent = 0);
    ~MainWindow();
    void setPath(char* path);

private:
    QWidget* centralWidget;
    QGridLayout* mainLayout;
    //properties and method for each section of screen
    QWidget* topbar;
    QWidget* makeTopBar();
    QWidget* leftbar;
    QWidget* makeLeftBar();
    QWidget* bottombar;
    QWidget* makeBottomBar();
    QWidget* rightside;
    QWidget* makeRightSide();
    //path of the executable
    QString expath;
};

#endif // MAINWINDOW_H
