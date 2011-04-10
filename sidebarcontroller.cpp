#include "sidebarcontroller.h"
#include "utilities.h"
#include <QtGui>
#include <QDebug>
#include "playlist.h"
#include <QContextMenuEvent>

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
  widget = makeWidget();
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
  int trimsize = 0;
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif

  playlistTableWidget = new QTableWidget(4, 1);

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

  playlistTableWidget->setObjectName("sideBarTopButtons");
  playlistTableWidget->setStyleSheet(util.getStylesheet());

  playlistTableWidget->setColumnWidth(0, 220);
  playlistTableWidget->setRowHeight(0, 25);
  playlistTableWidget->setRowHeight(1, 25);
  //playlistTableWidget->setRowHeight(2, 5);
  playlistTableWidget->setRowHeight(2, 25);

  playlistTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(playlistTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(Clicked(int,int)));
  QObject::connect(playlistTableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  QObject::connect(playlistTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));

  const LibraryController* LibCont = libpass;




  QTableWidgetItem* btns[4];
  btns[0] = new QTableWidgetItem("Display All Media");
  btns[0]->setFont(font);
  btns[0]->setFlags(btns[0]->flags() & (~Qt::ItemIsEditable));
  btns[0]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  btns[1] = new QTableWidgetItem("Display All Playlists");
  btns[1]->setFont(font);
  btns[1]->setFlags(btns[1]->flags() & (~Qt::ItemIsEditable));
  btns[1]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  btns[2] = new QTableWidgetItem("");
  btns[2]->setFlags(btns[2]->flags() & (~Qt::ItemIsEditable));
  btns[2]->setFlags(btns[2]->flags() & (~Qt::ItemIsSelectable));
  font.setBold(true);
  btns[3] = new QTableWidgetItem("Recent Playlists");
  btns[3]->setFont(font);
  btns[3]->setFlags(btns[3]->flags() & (~Qt::ItemIsEditable));
  btns[3]->setFlags(btns[3]->flags() & (~Qt::ItemIsSelectable));
  btns[3]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  playlistTableWidget->setItem(0, 0, btns[0]);
  playlistTableWidget->setItem(1, 0,btns[1]);
  //playlistTableWidget->setItem(2, 0,btns[2]);
  playlistTableWidget->setItem(2, 0,btns[3]);

  return playlistTableWidget;

  //QObject::connect(playlistTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));
  //displayTableWidget->setMaximumHeight(77);
  //QObject::connect(playlistTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));
  //playlistbarlayout = new QGridLayout(playlistbar);
  //playlistbarlayout->setColumnMinimumWidth(0, 220);
  //playlistbarlayout->setSpacing(0);
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
  trimsize = trimsize + 3;

  playlistTableWidget->setMaximumHeight(trimsize*25);   //////CAUSING ISSUES
  playlistTableWidget->setMinimumHeight(trimsize*25);
  playlistTableWidget->setRowCount(trimsize);
  int  i;

//  for(i = 3; i<(trimsize); i++)
//  {
//    QSqlRecord currecord = playlists.at(i-3);
//    typearray[i-3] = currecord.field(1).value().toBool();
//    QString name = currecord.field(0).value().toString();
//    QTableWidgetItem* item1 = new QTableWidgetItem(name);
//    item1->setFont(font);
//    item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
//    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//    playlistTableWidget->setItem(i, 0, item1);
//    playlistTableWidget->setRowHeight(i, 25);
//  }

  for(i = 3; i<(trimsize); i++)
  {
    QSqlRecord currecord = playlists->at(i-3);
    typearray[i-3] = currecord.field(1).value().toBool();
    QString name = currecord.field(0).value().toString();
    namearray[i-3] = name;
    QWidget* item1 = makePlaylistRow(name);
    playlistTableWidget->setRowHeight(i, 25);
    playlistTableWidget->setCellWidget(i, 0, item1);
  }
}


QWidget* SidebarController::makePlaylistRow(QString name)
{
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif

  QWidget *temp = new QWidget();

  temp->setMaximumHeight(25);
  QGridLayout* playlistTitleLayout = new QGridLayout(temp);
  playlistTitleLayout->setMargin(0);
  playlistTitleLayout->setSpacing(0);
  playlistTitleLayout->setColumnMinimumWidth(0, 30);

  QLabel* text = new QLabel(name);
  text->setFont(font);

  QFrame* sideborder = new QFrame();
  sideborder->setMaximumWidth(30);
  sideborder->setObjectName("sideBarPlaylistCell");
  temp->setStyleSheet(util.getStylesheet());

  playlistTitleLayout->addWidget(sideborder, 0, 0);
  playlistTitleLayout->addWidget(text, 0, 1, Qt::AlignLeft);
  return temp;
}

QWidget* SidebarController::getWidget()
{
  return widget;
};

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
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPreviewPane");
  temp->setStyleSheet(util.getStylesheet());
  QGridLayout* previewPaneLayout = new QGridLayout(temp);
  previewPaneLayout->setMargin(0);
  previewPaneLayout->setSpacing(0);
  QFrame* text = new QFrame(temp);
  text->setObjectName("sideBarPreviewPic");
  text->setStyleSheet(util.getStylesheet());
  text->setMinimumSize(139, 139);
  text->setMaximumSize(139, 139);
  previewPaneLayout->addWidget(text, 0,0, 2, 1,  Qt::AlignHCenter);

 /* QFrame* veil = new QFrame(temp);
  veil->setObjectName("sideBarVeilPic");
  veil->setStyleSheet(util.getStylesheet());
  veil->setMinimumSize(220, 25);
  veil->setMaximumSize(220, 25);
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
  timebarLayout->addWidget(timetext, 0,0, Qt::AlignHCenter);*/
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
  int row = playlistTableWidget->rowAt(main->mapToParent(pos).y());
  if(row >= 3)
  {
    bool type = typearray[row-3];
    QString text = namearray[row-3];
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

void SidebarController::Clicked(int row, int column)
{
  if(row == 0)
  {
    emit displayAllMedia();
  }
  else if(row == 1)
  {
    qDebug() << "View All Playlists";
  }
  else if(row >= 3)
  {
    bool type = typearray[row-3];
    QString text = namearray[row-3];
    Playlist pass(db, text);
    QList<QSqlRecord>* alltracks = pass.getAllTracks();
    emit(playthis(alltracks));
  }
}

void SidebarController::DoubleClicked(int row, int column)
{
  if(row >= 3)
  {
    qDebug() << "Playlist Playing";
  }
}
