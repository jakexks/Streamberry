#include "previewpane.h"
#include <QtGui>
#include "playlist.h"
#include <stdlib.h>
#include <QTableWidgetItem>

PreviewPane::PreviewPane(Utilities &utilities, Database& datab, LibraryController* lib)
{
  util = &utilities;
  db = &datab;
  libcont = lib;
  showing = 11;
  mainwidget = new QWidget();
  mainwidget->setObjectName("sideBarPreviewPane");
  mainwidget->setStyleSheet(util->getStylesheet());
  QGridLayout* previewPaneLayout = new QGridLayout(mainwidget);
  previewPaneLayout->setMargin(0);
  previewPaneLayout->setSpacing(0);
  window = new QStackedWidget(mainwidget);
  window->setMinimumSize(219, 220);
  window->setMaximumSize(219, 220);

 // window->setObjectName("sideBarPreviewPic");
 // window->setStyleSheet(util->getStylesheet());

  previewPaneLayout->addWidget(window, 0,0, 2, 1,  Qt::AlignHCenter);

  QWidget* default1 = makeDefault();
  QWidget* playlist = makePlaylist();
  QWidget* art = makeArt();

  window->addWidget(default1);
  window->addWidget(playlist);
  window->addWidget(art);

  state =0;

  QFrame* veil = new QFrame(mainwidget);
  veil->setObjectName("sideBarVeilPic");
  veil->setStyleSheet(util->getStylesheet());
  veil->setMinimumSize(219, 25);
  veil->setMaximumSize(219, 25);
  previewPaneLayout->addWidget(veil, 1, 0, Qt::AlignBottom);

  QGridLayout* timebarLayout = new QGridLayout(veil);
  timebarLayout->setMargin(0);
  timebarLayout->setSpacing(0);
  timetext = new QLabel("01:06 / 04:28");
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
  font.setBold(true);
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif
  timetext->setFont(font);
  timebarLayout->addWidget(timetext, 0,0, Qt::AlignHCenter);


}

///DEFAULT FUNCTIONS///

QWidget* PreviewPane::makeDefault()
{
  QWidget* pane = new QWidget(window);
  QGridLayout* PreviewPaneLayout = new QGridLayout(pane);
  QFrame* temp = new QFrame(pane);
  temp->setMinimumSize(139, 139);
  temp->setMaximumSize(139, 139);
  temp->setObjectName("sideBarPreviewPic");
  temp->setStyleSheet(util->getStylesheet());
  PreviewPaneLayout->addWidget(temp,0,0, (Qt::AlignHCenter | Qt::AlignVCenter));
  return pane;
}

void PreviewPane::rolloverDefault()
{
  state =0;
  window->setCurrentIndex(0);
}


///PLAYLIST PREVIEW FUNCTIONS///
QWidget* PreviewPane::makePlaylist()
{
  QWidget* pane = new QWidget(window);
  QGridLayout* PreviewPaneLayout = new QGridLayout(pane);
  tableframe = new QTableWidget(showing,2, mainwidget);

  tableframe->setShowGrid(false);
  tableframe->horizontalHeader()->setHighlightSections(false);
  tableframe->horizontalHeader()->setSortIndicatorShown(false);
  tableframe->setWordWrap(false);
  tableframe->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tableframe->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tableframe->verticalHeader()->hide();
  tableframe->horizontalHeader()->hide();
  tableframe->setSelectionMode(QAbstractItemView::NoSelection);
  tableframe->setFrameStyle(QFrame::Plain | QFrame::Plain);
  tableframe->setLineWidth(0);
  tableframe->setMidLineWidth(0);
  tableframe->setFocusPolicy(Qt::NoFocus);
  tableframe->setObjectName("libraryTableWidget");
  tableframe->setStyleSheet(util->getStylesheet());
  tableframe->setColumnWidth(0, 105);
  tableframe->setColumnWidth(1, 105);
  for(int i=0; i<showing;i++)
    tableframe->setRowHeight(i, 19);

  PreviewPaneLayout->setSpacing(0);
  PreviewPaneLayout->setMargin(5);
  PreviewPaneLayout->addWidget(tableframe,0,0);
  return pane;
}

void PreviewPane::rolloverPlaylist(QString name)
{
  updatePreview(name);
  state = 1;
  window->setCurrentIndex(1);
}

void PreviewPane::updatePreview(QString name)
{
  QColor background(util->getColor(1));
  QColor background2(util->getColor(2));
  QTableWidgetItem* temp1;
  QTableWidgetItem* temp2;
  for(int i= 0; i<showing; i++)
  {
    temp1 = tableframe->takeItem(i,0);
    temp2 = tableframe->takeItem(i,1);
    delete temp1;
    delete temp2;
  }
  Playlist temp(*db, name);
  QList<QSqlRecord>* tracks = temp.getAllTracks();
  QList<QSqlRecord> chosen;
  for(int i=0; i<showing && (tracks->size() > 0); i++)
  {
    int random = rand() % tracks->size();
    if(random == -1)
    {
      for(int j=0;j<10;j++)
      {
        random = rand() % tracks->size();
        if(random != -1)
          break;
      }
      break;
    }
    QSqlRecord temp(tracks->at(random));
    tracks->removeAt(random);
    chosen.append(temp);
  }
  delete tracks;

  for(int i=0; i<chosen.size(); i++)
  {

    QString title = chosen.at(i).field("Title").value().toString();
    temp1 =  new QTableWidgetItem(title, 0);
    tableframe->setItem(i,0, temp1);
    temp1->setFlags(temp1->flags() & (~Qt::ItemIsEditable));
    if(i%2==1)
      temp1->setBackgroundColor(background);
    else
      temp1->setBackgroundColor(background2);

    QString artist = chosen.at(i).field("Artist").value().toString();
    temp2 =  new QTableWidgetItem(artist, 0);
    temp2->setFlags(temp2->flags() & (~Qt::ItemIsEditable));
    tableframe->setItem(i,1, temp2);
    if(i%2==1)
      temp2->setBackgroundColor(background);
    else
      temp2->setBackgroundColor(background2);
  }
  for(int i= chosen.size(); i<showing; i++)
  {
    temp1 = tableframe->takeItem(i,0);
    temp2 = tableframe->takeItem(i,1);
    delete temp1;
    delete temp2;
  }
}

///ALBUM ART PREVIEW FUNCTIONS///
QWidget* PreviewPane::makeArt()
{
  QWidget* pane = new QWidget(window);
  QGridLayout* ArtPaneLayout = new QGridLayout(pane);
  pictureframe = new QLabel(pane);
  ArtPaneLayout->addWidget(pictureframe);
  return pane;
}


void PreviewPane::displayAlbumArt()
{
  QPixmap pic("defaultAlbumArt02_111.png",  "png");
  updateArt(pic);
  state = 2;
  window->setCurrentIndex(2);
}

void PreviewPane::displayAlbumArt(QPixmap pic)
{
  updateArt(pic);
  state = 2;
  window->setCurrentIndex(2);
}

void PreviewPane::updateArt(QPixmap pic)
{
  //qDebug() << "Album art fetch and change";
  pictureframe->setPixmap(pic);
  pictureframe->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //Load Required Picture into the picture frame
}

PreviewPane::~PreviewPane()
{
  delete mainwidget;
  delete tableframe;
  delete pictureframe;
}

QWidget* PreviewPane::getwidget()
{
  return mainwidget;
}
