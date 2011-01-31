#include "albumpanel.h"
#include <QGridLayout>
#include <QLabel>
#include <albuminfo.h>

AlbumPanel::AlbumPanel(QWidget *parent) :
    QWidget(parent)
{
    grid = new QGridLayout(parent);
    grid->setSpacing(0);
    grid->setMargin(0);
    QWidget *art = makeAlbumArt();
    grid->addWidget(art,0,0);
    albuminfo ai;
    grid->addWidget(new QLabel(ai.getTitle()),1,0);
    grid->addWidget(new QLabel(ai.getArtist()),2,0);
}

QWidget *AlbumPanel::makeAlbumArt() {
    QWidget *temp = new QWidget();
    temp->setFixedSize(100,100);
    QString style = "background-image: url(http://dl.dropbox.com/u/4381138/Folder.jpg); background-repeat: no-repeat; border:none;";
    temp->setStyleSheet(style);
    return temp;
}
