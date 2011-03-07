#include "songinfo.h"
#include "utilities.h"
#include <QtGui>
#include <QFontMetrics>
#include "QDebug"

SongInfo::SongInfo(Utilities &utilities) : util(utilities)
{
    widget = new QWidget();
    temp= new QGridLayout(widget);
    widget->setStyleSheet(util.getStylesheet());
    widget->setObjectName("songInfo");
    widget->setFixedSize(222,96);
    QFont font;

    font.setStyleHint(QFont::System, QFont::PreferAntialias);
    fontmetrics = new QFontMetrics(font);
    QLabel *line = new QLabel();
    line->setObjectName("songInfoLine");
    line->setStyleSheet(util.getStylesheet());
    line->setMaximumWidth(1);
    line->setMinimumHeight(70);
    QWidget* labels = settext(" ", " ", " ");
    temp->addWidget(line,0,0);
    temp->setAlignment(line,Qt::AlignRight);
    temp->addWidget(labels,0,1);
//    labels=settext("1","2","3");
//    temp->addWidget(labels,0,1);
  //temp->update();
}

SongInfo::~SongInfo()
{
    delete widget;
    delete fontmetrics;
}

QWidget* SongInfo::getWidget()
{
    return widget;
}

QWidget* SongInfo::settext(QString album, QString  artist, QString  song)
{
    QWidget *tempw = new QWidget();
    QGridLayout *temp = new QGridLayout(tempw);
    temp->setSpacing(0);
    temp->setMargin(0);

    album=fontmetrics->elidedText(album,Qt::ElideRight,180);
    artist=fontmetrics->elidedText(artist,Qt::ElideRight,180);
    song=fontmetrics->elidedText(song,Qt::ElideRight,180);

    QLabel *albumlabel= new QLabel(album);
    albumlabel->setMaximumSize(180,20);
    albumlabel->setMinimumSize(180,20);
    albumlabel->setObjectName("songInfoLabel");
    albumlabel->setStyleSheet(util.getStylesheet());
    QLabel *artistlabel= new QLabel(artist);
    artistlabel->setMaximumSize(180,20);
    artistlabel->setMinimumSize(180,20);
    artistlabel->setObjectName("songInfoLabel");
    artistlabel->setStyleSheet(util.getStylesheet());
    QLabel *songlabel= new QLabel(song);
    songlabel->setMaximumSize(180,20);
    songlabel->setMinimumSize(180,20);

    temp->addWidget(songlabel, 0, 1);
    temp->addWidget(artistlabel, 1, 1);
    temp->addWidget(albumlabel, 2, 1);
    return tempw;
}

void SongInfo::updatelabels(QString album, QString  artist, QString  song)
{
QWidget* labels = settext(album, artist, song);

 temp->addWidget(labels,0,1);
}
