#include "topbarcontroller.h"
#include "sbsearchbar.h"
#include "utilities.h"
#include <QtGui>
#include <QGridLayout>

TopbarController::TopbarController(Utilities &utilities) : util(utilities)
{
    expath = utilities.getExecutePath();
    widget = makeWidget();
}

QWidget* TopbarController::getWidget()
{
    return widget;
}

QWidget* TopbarController::makeWidget()
{
    QWidget *temp = new QWidget();
    temp->setObjectName("topBarWidget");

    //Grids to hold objects in
    QGridLayout *outer = new QGridLayout(temp);
    outer->setMargin(0);
    outer->setSpacing(0);
    QGridLayout *innerleft = new QGridLayout();
    innerleft->setMargin(0);
    innerleft->setSpacing(0);
    innerleft->setContentsMargins(0,2,0,0);
    QGridLayout *innermiddle = new QGridLayout();
    innermiddle->setMargin(0);
    innermiddle->setSpacing(0);
    innermiddle->setContentsMargins(0,2,0,0);
    QGridLayout *innerright = new QGridLayout();
    innerright->setMargin(0);
    innerright->setSpacing(0);

    //Buttons for menu
    QPushButton *back = new QPushButton();
    back->setObjectName("topBarBack");
    QPushButton *forward = new QPushButton();
    forward->setObjectName("topBarForward");
    QPushButton *music = new QPushButton();
    music->setObjectName("topBarMusic");
    QPushButton *videos = new QPushButton();
    videos->setObjectName("topBarVideos");
    SBSearchBar *search = new SBSearchBar(expath);

    //Split it into three sections
    outer->addLayout(innerleft, 1, 0);
    outer->addLayout(innermiddle, 0, 1, 3, 1);
    outer->addLayout(innerright, 1, 2);
    //set which rows and columns in outer layout are stretchy
    outer->setRowStretch(0, 1);
    outer->setRowStretch(1, 0);
    outer->setRowStretch(2, 1);
    outer->setColumnStretch(0, 0);
    outer->setColumnStretch(1, 1);
    outer->setColumnStretch(2, 0);
    //Set which are stretchy in inner layouts
    innermiddle->setColumnStretch(0, 1);
    innermiddle->setColumnStretch(1, 0);
    innermiddle->setColumnStretch(2, 0);
    innermiddle->setColumnStretch(3, 1);
    //Set measurements of spacer columns
    innerleft->setColumnMinimumWidth(0, 15);
    innerleft->setColumnMinimumWidth(2, 5);
    innermiddle->setColumnMinimumWidth(0, 40);
    innermiddle->setColumnMinimumWidth(3, 40);
    innerright->setColumnMinimumWidth(1, 15);
    //style the buttons
    back->setMaximumSize(35, 29);
    back->setMinimumSize(35, 29);
    back->setFlat(true);
    forward->setMaximumSize(35, 29);
    forward->setMinimumSize(35, 29);
    forward->setFlat(true);
    music->setMaximumSize(106, 33);
    music->setMinimumSize(106, 33);
    music->setFlat(true);
    videos->setMaximumSize(114, 33);
    videos->setMinimumSize(114, 33);
    videos->setFlat(true);
    //Add buttons to each section
    innerleft->addWidget(back, 0, 1);
    innerleft->addWidget(forward, 0, 3);
    innermiddle->addWidget(music, 0, 1);
    innermiddle->addWidget(videos, 0, 2);
    innerright->addWidget(search, 0, 0);

    return temp;
}
