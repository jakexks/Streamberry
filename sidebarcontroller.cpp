#include "sidebarcontroller.h"
#include "utilities.h"
#include <QtGui>
#include <QDebug>
#include "playlist.h"
#include <QContextMenuEvent>

#define TOPBARHEIGHT 26

SidebarController::SidebarController(Utilities &utilities, Database& datab, LibraryController& lib) : util(utilities), db(datab), libpass(lib)
{
  expath = utilities.getExecutePath();
  widget = makeWidget();
}

QWidget* SidebarController::makeWidget()
{
  QWidget* main = new QWidget();
  sidebarlayout = new QGridLayout(main);
  main->setMaximumWidth(220);
  main->setObjectName("sideBarMainWidget");
  main->setStyleSheet(util.getStylesheet());

  sidebarlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
  sidebarlayout->setRowStretch(0, 0);
  sidebarlayout->setRowStretch(1, 0);
  sidebarlayout->setRowStretch(2, 2);
  sidebarlayout->setRowStretch(3, 0);
  sidebarlayout->setRowStretch(4, 0);

  sidebarlayout->setRowMinimumHeight(5, 220);
  sidebarlayout->setColumnMinimumWidth(0, 220);
  sidebarlayout->setMargin(0);
  sidebarlayout->setSpacing(0);

  playlistbtn = makePlaylistBtn();
  topButtons = buildButtons();
  playlistbar = makePlaylistBar();
  previewbar = makePreviewBar();
  previewbtn = makePreviewBtn();

  smartmenu = new PlaylistMenu(true);
  normalmenu = new PlaylistMenu(false);

  sidebarlayout->addWidget(playlistbtn, 0, 0);
  sidebarlayout->addWidget(topButtons, 1, 0);
  sidebarlayout->addWidget(playlistbar, 2, 0);
  sidebarlayout->addWidget(previewbtn, 3, 0);
  sidebarlayout->addWidget(previewbar, 4, 0);

  playlistbarlayout->addWidget(tablewidget, 0, 0, Qt::AlignTop);

  return main;
}

QTableWidget* SidebarController::buildButtons()
{
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
  font.setPointSize(11);

  tablewidget = new QTableWidget(2, 1);
  tablewidget->setShowGrid(false);
  tablewidget->horizontalHeader()->setHighlightSections(false);
  tablewidget->horizontalHeader()->setSortIndicatorShown(false);
  tablewidget->setWordWrap(false);
  tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  tablewidget->setColumnWidth(0, 220);
  tablewidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tablewidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tablewidget->verticalHeader()->hide();
  tablewidget->setFrameStyle(QFrame::Plain | QFrame::Plain);
  tablewidget->setLineWidth(0);
  tablewidget->setMidLineWidth(0);
  tablewidget->setFocusPolicy(Qt::NoFocus);
  tablewidget->horizontalHeader()->hide();
  tablewidget->setObjectName("sideBarTopButtons");
  QObject::connect(tablewidget, SIGNAL(cellClicked(int,int)), this, SLOT(Clicked(int,int)));
  tablewidget->setStyleSheet(util.getStylesheet());

  QTableWidgetItem* item = new QTableWidgetItem("Display All Media");
  item->setFont(font);
  item->setFlags(item->flags() & (~Qt::ItemIsEditable));
  item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  tablewidget->setItem(0, 0, item);

  item = new QTableWidgetItem("Display All Playlists");
  item->setFont(font);
  item->setFlags(item->flags() & (~Qt::ItemIsEditable));
  item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  tablewidget->setItem(1, 0, item);

  return tablewidget;
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
  return temp;
}

//QPushButton* SidebarController::makeAllMediaBtn()
//{
//  QPushButton* temp = new QPushButton();
//  temp->setObjectName("sideBarAllMediaButton");
//  temp->setMaximumSize(220, 35);
//  temp->setMinimumSize(220, 35);
//  temp->setFlat(true);
//  temp->setStyleSheet(util.getStylesheet());
//  return temp;
//}

//QPushButton* SidebarController::makeAllPlaylistsBtn()
//{
//  QPushButton* temp = new QPushButton();
//  temp->setObjectName("sideBarAllPlaylistsButton");
//  temp->setMaximumSize(220, 26);
//  temp->setMinimumSize(220, 26);
//  temp->setFlat(true);
//  temp->setStyleSheet(util.getStylesheet());
//  return temp;
//}

QWidget* SidebarController::makePlaylistBtn()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPlaylistsTitle");
  temp->setStyleSheet(util.getStylesheet());
  return temp;
}

QWidget* SidebarController::makePreviewBar()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPreviewPane");
  temp->setStyleSheet(util.getStylesheet());
  return temp;
}

QWidget* SidebarController::makePlaylistBar()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPlaylistsPane");
  temp->setStyleSheet(util.getStylesheet());
  buildplaylistbar(temp);

  QString sizestring = db.getSetting("windowSize");
  if( sizestring.compare("") )
    updateplaylistbar( 7 );
  else
  {
    double size = sizestring.split("|").at(1).toDouble();
    updateplaylistbar( (int)(size/89.25) );
  }
  return temp;
}

void SidebarController::updateplaylistbar(int shownumber)
{
  int trimsize = shownumber;
  QList<QSqlRecord> playlists = db.getAllPlaylists();
  if(playlists.size() > trimsize)
  {
    int p ;
    for(p = playlists.size(); p > trimsize; p-- )
      playlists.removeLast();
  }
  else
  {
    trimsize = playlists.size();
  }
  tablewidget->setMaximumHeight(trimsize*30);
  tablewidget->setRowCount(trimsize);
  int  i;

  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
  font.setPointSize(11);

  for(i = 0; i<(trimsize); i++)
  {
    QSqlRecord currecord = playlists.at(i);
    typearray[i] = currecord.field(1).value().toBool();
    QString name = currecord.field(0).value().toString();
    QTableWidgetItem* item = new QTableWidgetItem(name);

    item->setFont(font);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tablewidget->setItem(i, 0, item);
  }
}

void SidebarController::buildplaylistbar(QWidget* playlistbar)
{
  int trimsize = 5;
  playlistbarlayout = new QGridLayout(playlistbar);
  playlistbarlayout->setColumnMinimumWidth(0, 220);
  playlistbarlayout->setSpacing(0);

  tablewidget = new QTableWidget(trimsize, 1, playlistbar);
  tablewidget->setShowGrid(false);
  tablewidget->horizontalHeader()->setHighlightSections(false);
  tablewidget->horizontalHeader()->setSortIndicatorShown(false);
  tablewidget->setWordWrap(false);
  tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  tablewidget->setColumnWidth(0, 203);
  tablewidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tablewidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  tablewidget->verticalHeader()->hide();
  tablewidget->setFrameStyle(QFrame::Plain | QFrame::Plain);

  tablewidget->setLineWidth(0);
  tablewidget->setMidLineWidth(0);
  tablewidget->setFocusPolicy(Qt::NoFocus);
  tablewidget->horizontalHeader()->hide();

  //QObject::connect(tablewidget, SIGNAL(cellClicked(int,int)), this, SLOT(Clicked(int,int)));
  QObject::connect(tablewidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));

  tablewidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(tablewidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));

  playlistbarlayout->addWidget(tablewidget, 0, 0, Qt::AlignTop);
  tablewidget->setObjectName("sideBarPlaylistTableWidget");
  tablewidget->setStyleSheet(util.getStylesheet());
}

void SidebarController::ShowContextMenu(const QPoint& pos)
{
  Playlist pass(db, (tablewidget->itemAt(pos)->text()));
  bool type = typearray[tablewidget->rowAt((mapToGlobal(pos).y()))];
  if(type == true)
    smartmenu->playlistrightclicked(&pass, &libpass);
  else
    normalmenu->playlistrightclicked(&pass, &libpass);
  QString sizestring = db.getSetting("windowSize");
  double size = sizestring.split("|").at(1).toDouble();
  updateplaylistbar( (int)(size/89.25) );
}


void SidebarController::DoubleClicked(int row, int column)
{
  QString test1;
  test1.setNum(row);
  QString test = "Playlist ";
  test += test1;
  test +=" has been double clicked";
  qDebug() << test;
}

void SidebarController::Clicked(int row, int column)
{
  QString test1;
  test1.setNum(row);
  QString test = "Button ";
  test += test1;
  test +=" has been clicked";
  qDebug() << test;
}




