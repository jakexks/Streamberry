#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
};

#endif // MAINWINDOW_H
