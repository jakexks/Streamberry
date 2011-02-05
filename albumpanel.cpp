#include "albumpanel.h"
#include <QGridLayout>
#include <QWidget>
#include <QColor>

AlbumPanel::AlbumPanel(QString path, QWidget *parent) :
    QWidget(parent)
{
    expath = path;
    grid = new QGridLayout(this);
    grid->setSpacing(0);
    grid->setMargin(15);
    QWidget *art = makeAlbumArt();
    grid->addWidget(art,0,0);
    grid->setRowStretch(0, 0);
    grid->setRowStretch(1, 0);
    grid->setRowStretch(2, 0);
    grid->setRowStretch(3, 1);
}

QWidget *AlbumPanel::makeAlbumArt() {
    QWidget *artwork = new QWidget();
    artwork->setFixedSize(100,100);
    artwork->setStyleSheet("background-color:green;");
    return artwork;
}
