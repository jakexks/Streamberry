#include "sidebarcontroller.h"
#include "utilities.h"
#include <QtGui>
#include <QDebug>
#include "playlist.h"
#include "librarycontroller.h"
#include <QContextMenuEvent>
#include <previewpane.h>
#include <QFrame>

#define TOPBARHEIGHT 26



/*
 TO DO:
 Highlight the playlist, button or whatever when you are viewing it in the main pane
 Link up the buttons

 grainy title images
 text size
 */

SidebarController::SidebarController(Utilities &utilities, Database& datab, LibraryController* lib) : util(utilities), db(datab), libpass(lib)
{
  libpass = lib;
  widget = makeWidget();
  playlistTableWidget->clearSelection();
  playlistTableWidget->setRangeSelected(QTableWidgetSelectionRange(0, 0, 0, 0), true);
  QObject::connect(this, SIGNAL(playplaylist(QString)), libpass, SLOT(playplaylist(QString)));
  QObject::connect(this, SIGNAL(playsmartplaylist(QString)), libpass, SLOT(playsmartplaylist(QString)));
}

SidebarController::~SidebarController()
{
  delete preview;
  delete main;
  delete smartmenu;
  delete normalmenu;
  delete playlistTableWidget;
}

QWidget* SidebarController::makeWidget()
{
  main = new QWidget();
  main->setMaximumWidth(220);
  main->setObjectName("sideBarMainWidget");
  main->setStyleSheet(util.getStylesheet());
  sidebarlayout = new QGridLayout(main);

  sidebarlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
  sidebarlayout->setRowMinimumHeight(3, TOPBARHEIGHT);
  sidebarlayout->setRowMinimumHeight(4, 220);
  sidebarlayout->setColumnMinimumWidth(0, 220);
  sidebarlayout->setMargin(0);
  sidebarlayout->setSpacing(0);

  playlistbtn = makePlaylistBtn();
  playlistbar = buildplaylistbar();
  previewbtn = makePreviewBtn();
  previewbar = makePreviewBar();
  Utilities* utilpoint = &util;
  smartmenu = new PlaylistMenu(true, utilpoint, db);
  normalmenu = new PlaylistMenu(false, utilpoint, db);

  sidebarlayout->setRowStretch(0, 0);
  sidebarlayout->setRowStretch(1, 0);
  sidebarlayout->setRowStretch(2, 1);
  sidebarlayout->setRowStretch(3, 0);
  sidebarlayout->setRowStretch(4, 0);
  //updateplaylistbar(7);
  sidebarlayout->addWidget(playlistbtn, 0, 0);

  sidebarlayout->addWidget(playlistbar, 1, 0);
  sidebarlayout->addWidget(previewbtn, 3, 0);
  sidebarlayout->addWidget(previewbar, 4, 0);

  return main;
}

QTableWidget* SidebarController::buildplaylistbar()
{
  //  int trimsize = 0;
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(9);
#else
  font.setPointSize(11);
#endif
  playlistTableWidget = new QTableWidget(5, 1);

  playlistTableWidget->setShowGrid(false);
  playlistTableWidget->horizontalHeader()->setHighlightSections(false);
  playlistTableWidget->horizontalHeader()->setSortIndicatorShown(false);
  playlistTableWidget->setWordWrap(false);
  playlistTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  playlistTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  playlistTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  playlistTableWidget->verticalHeader()->hide();
  playlistTableWidget->horizontalHeader()->hide();
  playlistTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  playlistTableWidget->setFrameStyle(QFrame::Plain | QFrame::Plain);
  playlistTableWidget->setLineWidth(0);
  playlistTableWidget->setMidLineWidth(0);
  playlistTableWidget->setFocusPolicy(Qt::NoFocus);
  playlistTableWidget->setMouseTracking(true);

  playlistTableWidget->setObjectName("sideBarTopButtons");
  playlistTableWidget->setStyleSheet(util.getStylesheet());

  playlistTableWidget->setColumnWidth(0, 220);
  playlistTableWidget->setRowHeight(0, 25);
  playlistTableWidget->setRowHeight(1, 25);
  playlistTableWidget->setRowHeight(2, 25);
  playlistTableWidget->setRowHeight(3, 25);

  playlistTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(playlistTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(Clicked(int,int)));
  QObject::connect(playlistTableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  QObject::connect(playlistTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));

  QObject::connect(playlistTableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(RolloverCell(int,int)));


  QTableWidgetItem* btns[4];

  btns[0] = new QTableWidgetItem("Display All Media");
  btns[0]->setFont(font);
  btns[0]->setFlags(btns[0]->flags() & (~Qt::ItemIsEditable));
  btns[0]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  btns[1] = new QTableWidgetItem("Display All Playlists");
  btns[1]->setFont(font);
  btns[1]->setFlags(btns[1]->flags() & (~Qt::ItemIsEditable));
  btns[1]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  btns[2] = new QTableWidgetItem("Display Video");
  btns[2]->setFont(font);
  btns[2]->setFlags(btns[2]->flags() & (~Qt::ItemIsEditable));
  btns[2]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  font.setBold(true);
  btns[3] = new QTableWidgetItem("Recent Playlists");
  btns[3]->setFont(font);
  btns[3]->setFlags(btns[3]->flags() & (~Qt::ItemIsEditable));
  btns[3]->setFlags(btns[3]->flags() & (~Qt::ItemIsSelectable));
  btns[3]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  playlistTableWidget->setItem(0, 0, btns[0]);
  playlistTableWidget->setItem(1, 0,btns[1]);
  playlistTableWidget->setItem(2, 0,btns[2]);
  playlistTableWidget->setItem(3, 0,btns[3]);

  return playlistTableWidget;
}

void SidebarController::updateplaylistbar(int shownumber)
{
  int trimsize = shownumber;
  QList<QSqlRecord>* playlists = db.getAllPlaylists();
  if(playlists->size() > trimsize)
  {
    int p ;
    for(p = playlists->size(); p > trimsize; p-- )
      playlists->removeLast();
  }
  else
  {
    trimsize = playlists->size();
  }
  trimsize = trimsize + 4;
  playlistTableWidget->setMaximumHeight(trimsize*30);   //////CAUSING ISSUES
  playlistTableWidget->setMinimumHeight(trimsize*30);
  playlistTableWidget->setRowCount(trimsize);
  int  i;

  for(i = 4; i<(trimsize); i++)
  {
    QSqlRecord currecord = playlists->at(i-4);
    typearray[i-4] = currecord.field(1).value().toBool();
    QString name = currecord.field(0).value().toString();
    namearray[i-4] = name;
    QWidget* item1 = makePlaylistRow(name, playlistTableWidget);
    playlistTableWidget->setRowHeight(i, 25);
    playlistTableWidget->setCellWidget(i, 0, item1);
  }
}




QWidget* SidebarController::makePlaylistRow(QString name, QWidget* parent)
{
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif

  QWidget* temp = new QWidget(parent);
  temp->setMouseTracking(true);
  temp->setMaximumHeight(25);
  QGridLayout* playlistTitleLayout = new QGridLayout(temp);
  playlistTitleLayout->setMargin(0);
  playlistTitleLayout->setSpacing(0);
  playlistTitleLayout->setColumnMinimumWidth(0, 30);
  playlistTitleLayout->setColumnStretch(0,0);


  QLabel* text = new QLabel(name,temp);
  text->setFont(font);
  text->setMouseTracking(true);
  //QFrame* sideborder = new QFrame(temp);
  //sideborder->setMaximumWidth(30);
  //sideborder->setObjectName("sideBarPlaylistCell");
  temp->setStyleSheet(util.getStylesheet());

  //playlistTitleLayout->addWidget(sideborder, 0, 0);
  playlistTitleLayout->addWidget(text, 0, 1, Qt::AlignLeft);

  return temp;
}

QWidget* SidebarController::getWidget()
{
  return widget;
}

QWidget* SidebarController::makePreviewBtn()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPreviewTitle");
  temp->setStyleSheet(util.getStylesheet());
  QGridLayout* previewTitleLayout = new QGridLayout(temp);
  previewTitleLayout->setMargin(0);
  previewTitleLayout->setSpacing(0);
  previewTitleLayout->setRowMinimumHeight(0, 26);
  previewTitleLayout->setColumnMinimumWidth(0, 64);
  QFrame* text = new QFrame(temp);
  text->setObjectName("sideBarPreviewText");
  text->setStyleSheet(util.getStylesheet());
  text->setMinimumSize(64, 26);
  text->setMaximumSize(64, 26);
  previewTitleLayout->addWidget(text, 0, 0);
  return temp;
}

QWidget* SidebarController::makePreviewBar()
{
  preview = new PreviewPane(util,db,libpass);
  QWidget* temp = preview->getwidget();
  QObject::connect(this, SIGNAL(rolledover(QString)), preview, SLOT(rolloverPlaylist(QString)));
  QObject::connect(this, SIGNAL(rolldefault()), preview, SLOT(rolloverDefault()));
  QObject::connect(libpass, SIGNAL(rolldefault()), preview, SLOT(rolloverDefault()));
  QObject::connect(libpass, SIGNAL(rollAlbum()), preview, SLOT(displayAlbumArt()));
  return temp;




}

QWidget* SidebarController::makePlaylistBtn()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPlaylistsTitle");
  temp->setStyleSheet(util.getStylesheet());
  QGridLayout* playlistTitleLayout = new QGridLayout(temp);
  playlistTitleLayout->setMargin(0);
  playlistTitleLayout->setSpacing(0);
  playlistTitleLayout->setRowMinimumHeight(0, 26);
  playlistTitleLayout->setColumnMinimumWidth(0, 64);
  QFrame* text = new QFrame(temp);
  text->setObjectName("sideBarPlaylistsText");
  text->setStyleSheet(util.getStylesheet());
  text->setMinimumSize(64, 26);
  text->setMaximumSize(64, 26);
  playlistTitleLayout->addWidget(text, 0, 0, Qt::AlignHCenter);

  QFrame* tinything = new QFrame(temp);
  tinything->setObjectName("sideBarDivided");
  tinything->setStyleSheet(util.getStylesheet());
  tinything->setMinimumSize(2, 26);
  tinything->setMaximumSize(2, 26);
  playlistTitleLayout->addWidget(tinything, 0, 0, Qt::AlignRight);
  return temp;
}


void SidebarController::ShowContextMenu(const QPoint& pos)
{
  int row = playlistTableWidget->rowAt((main->mapToParent(pos).y()));
  if(row >= 4)
  {
    bool type = typearray[row-4];
    QString text = namearray[row-4];
    Playlist pass(db, text);
    if(type == true)
      smartmenu->playlistrightclicked(&pass, libpass);
    else
      normalmenu->playlistrightclicked(&pass, libpass);

    QString sizestring = db.getSetting("windowSize");
    if( sizestring=="" )
      updateplaylistbar( 7 );
    else
    {
      double size = sizestring.split("|").at(1).toDouble();
      updateplaylistbar( (int)(size/89.25) );
    }
  }
}

void SidebarController::Clicked(int row, int)
{
  if(row == 0)
  {
    libpass->pushAllView();
  }
  else if(row == 1)
  {
    qDebug() << "View All Playlists";
  }
  else if(row == 2)
  {
    qDebug() << "Display Video";
  }
  else if(row >= 4)
  {
    //    bool type = typearray[row-3];
    QString text = namearray[row-4];
    Playlist pass(db, text);
    if(pass.getPlaylistType()==1)
    {
      libpass->pushSmartPlaylist(text, pass.getFilter());
    }
    else
    {
      libpass->pushNormalPlaylist(text);
    }

    //QList<QSqlRecord>* alltracks = pass.getAllTracks();
    //emit(playthis(alltracks));
  }

}

void SidebarController::DoubleClicked(int row, int)
{
  if(row >= 4)
  {
    QString name = namearray[row-4];
    bool type = typearray[row-4];
    qDebug() << name << " Playing" << type;
    if(type == true)
      emit playsmartplaylist(name);
    else
      emit playplaylist(name);
  }
}


void SidebarController::RolloverCell(int row,int)
{
  if(row >= 4)
  {
    QString name = namearray[row-4];
    emit rolledover(name);
  }
  else
    emit rolldefault();
}

void SidebarController::setSelectedPlaylist(QString name)
{
  // if empty select first row
  if(name=="")
  {
    playlistTableWidget->clearSelection();
    playlistTableWidget->setRangeSelected(QTableWidgetSelectionRange(0, 0, 0, 0), true);
  }
  else
  {
    int i = 0;
    while(i<playlistTableWidget->rowCount()-4 && i<=15)
    {
      if(namearray[i]==name)
      {
        playlistTableWidget->clearSelection();
        playlistTableWidget->setRangeSelected(QTableWidgetSelectionRange(i+4, 0, i+4, 0), true);
        break;
      }

      i++;
    }
  }
}
