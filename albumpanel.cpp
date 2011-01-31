#include "albumpanel.h"
#include <QGridLayout>
#include <QLabel>
#include <albuminfo.h>

AlbumPanel::AlbumPanel(QString path, QWidget *parent) :
    QWidget(parent)
{
    expath = path;
    grid = new QGridLayout(this);
    grid->setSpacing(0);
    grid->setMargin(15);
    QWidget *art = makeAlbumArt();
    grid->addWidget(art,0,0);
    albuminfo ai;
    grid->addWidget(new QLabel(ai.getTitle()),1,0);
    grid->addWidget(new QLabel(ai.getArtist()),2,0);
    grid->setRowStretch(0, 0);
    grid->setRowStretch(1, 0);
    grid->setRowStretch(2, 0);
    grid->setRowStretch(3, 1);
}

QWidget *AlbumPanel::makeAlbumArt() {
    QWidget *temp = new QWidget();
    temp->setFixedSize(100,100);
    QString style = "background-image: url(";
    style += expath;
    style += "images/shotters.jpg); background-repeat: no-repeat; border:none;";
    temp->setStyleSheet(style);
    return temp;
}
