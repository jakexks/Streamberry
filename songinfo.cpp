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

    line = new QFrame();
    line->setObjectName("songInfoLine");
    line->setStyleSheet(util.getStylesheet());
    line->hide();
    line->setFixedSize(1, 70);

    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setMargin(0);

    albumlabel= new QLabel();
    artistlabel= new QLabel();
    titlelabel= new QLabel();

    albumlabel->setMaximumSize(180,20);
    albumlabel->setMinimumSize(180,20);
    albumlabel->setObjectName("songInfoLabelBold");
    albumlabel->setStyleSheet(util.getStylesheet());

    artistlabel->setMaximumSize(180,20);
    artistlabel->setMinimumSize(180,20);
    artistlabel->setObjectName("songInfoLabelBold");
    artistlabel->setStyleSheet(util.getStylesheet());

    titlelabel->setObjectName("songInfoLabel");
    titlelabel->setStyleSheet(util.getStylesheet());
    titlelabel->setMaximumSize(180,20);
    titlelabel->setMinimumSize(180,20);

    layout->addWidget(titlelabel, 0, 1);
    layout->addWidget(artistlabel, 1, 1);
    layout->addWidget(albumlabel, 2, 1);

    temp->addLayout(layout, 0, 1);
    temp->setAlignment(line, Qt::AlignVCenter | Qt::AlignRight);
    temp->addWidget(line, 0, 0);
}

QWidget* SongInfo::getWidget()
{
    return widget;
}

void SongInfo::setText(QString album, QString artist, QString title, QString track)
{
    if(track!="")
    {
        track += ". ";
        track += title;
        title = track;
    }

    if(album!="" || artist!="" || title!="" || track!="")
    {
        line->show();
    }
    else
    {
        line->hide();
    }

    QFont font;
    font.setStyleHint(QFont::System, QFont::PreferAntialias);
    font.setBold(true);
    QFontMetrics fontmetrics(font);

    album=fontmetrics.elidedText(album,Qt::ElideRight,180);
    artist=fontmetrics.elidedText(artist,Qt::ElideRight,180);
    title=fontmetrics.elidedText(title,Qt::ElideRight,180);

    albumlabel->setText(album);
    artistlabel->setText(artist);
    titlelabel->setText(title);
}

void SongInfo::updateLabels(QString album, QString artist, QString title, QString track)
{
    setText(album, artist, title, track);

}
