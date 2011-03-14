#include "albumpanel.h"
#include <QGridLayout>
#include <QWidget>
#include <QColor>


AlbumPanel::AlbumPanel(Utilities& utilities, QString path, QWidget *parent) :
    util(utilities),QWidget(parent)
{
    expath = path;
    grid = new QGridLayout(this);
    grid->setSpacing(0);
    grid->setMargin(8);
    QWidget *art = makeAlbumArt();
    grid->addWidget(art,0,0);
    grid->setRowStretch(0, 0);
    grid->setRowStretch(1, 0);
    grid->setRowStretch(2, 0);
    grid->setRowStretch(3, 1);
}

QWidget *AlbumPanel::makeAlbumArt() {
    QWidget *artwork = new QWidget();
    artwork->setFixedSize(111,111);
    artwork->setObjectName("AlbumArtPicture");
    artwork->setStyleSheet(util.getStylesheet());
    return artwork;
}
