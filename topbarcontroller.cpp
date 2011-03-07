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
    temp->setStyleSheet(util.getStylesheet());
    //Grids to hold objects in
    QGridLayout *outer = new QGridLayout(temp);
    outer->setMargin(0);
    outer->setSpacing(0);
    QGridLayout *separatorLayout = new QGridLayout();
    separatorLayout->setMargin(0);
    separatorLayout->setSpacing(0);
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

//    separator
    QLabel *separator = new QLabel();
    separator->setObjectName("topBarSeparatorRight");
    separator->setStyleSheet(util.getStylesheet());
    //Buttons for menu
    QPushButton *back = new QPushButton();
    back->setObjectName("topBarBack");
    back->setStyleSheet(util.getStylesheet());
    QPushButton *forward = new QPushButton();
    forward->setObjectName("topBarForward");
    forward->setStyleSheet(util.getStylesheet());
    music= new QPushButton();
    music->setObjectName("topBarMusic");
    music->setStyleSheet(util.getStylesheet());
    videos = new QPushButton();
    videos->setObjectName("topBarVideos");
    videos->setStyleSheet(util.getStylesheet());
    search = new SBSearchBar(expath);

    //Split it into three sections
    outer->addLayout(separatorLayout, 1, 0);
    outer->addLayout(innerleft, 1, 1);
    outer->addLayout(innermiddle, 0, 2, 3, 1);
    outer->addLayout(innerright, 1, 3);
    //set which rows and columns in outer layout are stretchy
    outer->setRowStretch(0, 1);
    outer->setRowStretch(1, 0);
    outer->setRowStretch(2, 1);
    outer->setColumnStretch(0, 0);
    outer->setColumnStretch(1, 1);
    outer->setColumnStretch(2, 0);
    //Set which are stretchy in inner layouts
    innermiddle->setColumnStretch(0, 244);
    innermiddle->setColumnStretch(1, 0);
    innermiddle->setColumnStretch(2, 164);
    //Set measurements of spacer columns
    separatorLayout->setColumnMinimumWidth(0,1);
    innerleft->setColumnMinimumWidth(0, 35);
    innermiddle->setColumnMinimumWidth(0, 35);
    innermiddle->setColumnMinimumWidth(1, 140);
    innermiddle->setColumnMinimumWidth(2, 24);
    innerright->setColumnMinimumWidth(1, 35);
    //style the buttons
    back->setMaximumSize(26, 24);
    back->setMinimumSize(26, 24);
    back->setFlat(true);
    forward->setMaximumSize(26, 24);
    forward->setMinimumSize(26, 24);
    forward->setFlat(true);
    music->setMaximumSize(140, 24);
    music->setMinimumSize(140, 24);
    music->setFlat(true);
    videos->setMaximumSize(140, 24);
    videos->setMinimumSize(69, 24);
    videos->setFlat(true);
    separator->setMinimumSize(1,26);
    separator->setMaximumSize(1,26);
    //Add buttons to each section
    separatorLayout->addWidget(separator, 0, 0);
    innerleft->addWidget(back, 0, 2);
    innerleft->addWidget(forward, 0, 3);
    innermiddle->addWidget(music, 0, 1);
    innermiddle->addWidget(videos, 0, 1);
    innermiddle->setAlignment(videos,Qt::AlignRight);
    innerright->addWidget(search, 0, 0);
    musicState=0;
    videosState=0;
    connect(music, SIGNAL(clicked()), this, SLOT(musicButtonControl()));
    connect(videos, SIGNAL(clicked()), this, SLOT(videosButtonControl()));
    return temp;
}


SBSearchBar* TopbarController::getSearchbar()
{
    return search;
}

void TopbarController::musicButtonControl()
{
    videosState=0;
    if(musicState==0)
    {
        music->setObjectName("topBarMusicchecked");
        music->setStyleSheet(util.getStylesheet());
        musicState=1;
    }
    else
    {
        music->setObjectName("topBarMusic");
        music->setStyleSheet(util.getStylesheet());
        musicState=0;
    }

}

void TopbarController::videosButtonControl()
{
   // qDebug()<<videos->checkStateSet ();
    musicState=0;
    if(videosState==0)
    {
        music->setObjectName("topBarVideoschecked");
        music->setStyleSheet(util.getStylesheet());
        videosState=1;
    }
    else
    {
        music->setObjectName("topBarMusic");
        music->setStyleSheet(util.getStylesheet());
        videosState=0;
    }
}
