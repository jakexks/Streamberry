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
  main = new QWidget();
  main->setMaximumWidth(220);
  main->setObjectName("sideBarMainWidget");
  main->setStyleSheet(util.getStylesheet());
  sidebarlayout = new QGridLayout(main);

  sidebarlayout->setRowMinimumHeight(0, TOPBARHEIGHT);
  sidebarlayout->setRowMinimumHeight(1, 60);
  sidebarlayout->setRowMinimumHeight(4, TOPBARHEIGHT);
  sidebarlayout->setRowMinimumHeight(5, 220);
  sidebarlayout->setColumnMinimumWidth(0, 220);
  sidebarlayout->setMargin(0);
  sidebarlayout->setSpacing(0);

  playlistbtn = makePlaylistBtn();
  topButtons = buildButtons();
  playlistbar = buildplaylistbar();
  previewbtn = makePreviewBtn();
  previewbar = makePreviewBar();
  smartmenu = new PlaylistMenu(true);
  normalmenu = new PlaylistMenu(false);

  sidebarlayout->setRowStretch(0, 0);
  sidebarlayout->setRowStretch(1, 0);
  sidebarlayout->setRowStretch(2, 0);
  sidebarlayout->setRowStretch(3, 1);
  sidebarlayout->setRowStretch(4, 0);
  sidebarlayout->setRowStretch(5, 0);

  //updateplaylistbar(7);

  sidebarlayout->addWidget(playlistbtn, 0, 0);
  sidebarlayout->addWidget(topButtons, 1, 0);
  sidebarlayout->addWidget(playlistbar, 2, 0);
  sidebarlayout->addWidget(previewbtn, 4, 0);
  sidebarlayout->addWidget(previewbar, 5, 0);
  return main;
}

QTableWidget* SidebarController::buildButtons()
{
  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif


  displayTableWidget = new QTableWidget(4, 1);
  displayTableWidget->setShowGrid(false);
  displayTableWidget->horizontalHeader()->setHighlightSections(false);
  displayTableWidget->horizontalHeader()->setSortIndicatorShown(false);
  displayTableWidget->setWordWrap(false);
  displayTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  displayTableWidget->setColumnWidth(0, 220);
  displayTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  displayTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  displayTableWidget->verticalHeader()->hide();
  displayTableWidget->setFrameStyle(QFrame::Plain | QFrame::Plain);
  displayTableWidget->setLineWidth(0);
  displayTableWidget->setRowHeight(0, 34);
  displayTableWidget->setRowHeight(1, 34);
  displayTableWidget->setRowHeight(2, 7);
  displayTableWidget->setRowHeight(3, 20);
  displayTableWidget->setMidLineWidth(0);
  displayTableWidget->setMaximumHeight(96);
  displayTableWidget->setFocusPolicy(Qt::NoFocus);
  displayTableWidget->horizontalHeader()->hide();
  displayTableWidget->setObjectName("sideBarTopButtons");
  QObject::connect(displayTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(Clicked(int,int)));
  displayTableWidget->setStyleSheet(util.getStylesheet());

  QTableWidgetItem* btns[4];
  btns[0] = new QTableWidgetItem("Display All Media");
  btns[0]->setFont(font);
  btns[0]->setFlags(btns[0]->flags() & (~Qt::ItemIsEditable));
  btns[0]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  displayTableWidget->setItem(0, 0, btns[0]);

  btns[1] = new QTableWidgetItem("Display All Playlists");
  btns[1]->setFont(font);
  btns[1]->setFlags(btns[1]->flags() & (~Qt::ItemIsEditable));
  btns[1]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  displayTableWidget->setItem(1, 0,btns[1]);

  btns[2] = new QTableWidgetItem("");
  btns[2]->setFlags(btns[2]->flags() & (~Qt::ItemIsEditable));
  displayTableWidget->setItem(2, 0,btns[2]);

  font.setBold(true);
  btns[3] = new QTableWidgetItem("Recent Playlists");
  btns[3]->setFont(font);
  btns[3]->setFlags(btns[3]->flags() & (~Qt::ItemIsEditable));
  btns[3]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  displayTableWidget->setItem(3, 0,btns[3]);

  return displayTableWidget;
}

QWidget* SidebarController::getWidget()
{
  return widget;
};

QWidget* SidebarController::makePreviewBtn()
{
  QWidget *temp = new QWidget();
  temp->setObjectName("sideBarPlaylistsTitle");
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

  QFrame* veil = new QFrame(temp);
  veil->setObjectName("sideBarVeilPic");
  veil->setStyleSheet(util.getStylesheet());
  veil->setMinimumSize(220, 25);
  veil->setMaximumSize(220, 25);
  previewPaneLayout->addWidget(veil, 1, 0, Qt::AlignBottom);

  QGridLayout* timebarLayout = new QGridLayout(veil);
  timebarLayout->setMargin(0);
  timebarLayout->setSpacing(0);
  timetext = new QLabel("01:06 / 04:28");
  timebarLayout->addWidget(timetext, 0,0, Qt::AlignHCenter);
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
  playlistTitleLayout->addWidget(text, 0, 0);
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
  playlistTableWidget->setMaximumHeight(trimsize*25);   //////CAUSING ISSUES
  playlistTableWidget->setRowCount(trimsize);
  int  i;

  QFont font;
  font.setStyleHint(QFont::System, QFont::PreferAntialias);  //STYLESHEET THIS!!!
#ifdef Q_WS_WIN
  font.setPointSize(10);
#else
  font.setPointSize(11);
#endif
  for(i = 0; i<(trimsize); i++)
  {
    QSqlRecord currecord = playlists.at(i);
    typearray[i] = currecord.field(1).value().toBool();
    QString name = currecord.field(0).value().toString();
    QTableWidgetItem* item1 = new QTableWidgetItem(name);
    item1->setFont(font);
    item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    playlistTableWidget->setItem(i, 0, item1);
    playlistTableWidget->setRowHeight(i, 25);
  }
}

QTableWidget* SidebarController::buildplaylistbar()
{
  int trimsize = 0;
  //playlistbarlayout = new QGridLayout(playlistbar);
  //playlistbarlayout->setColumnMinimumWidth(0, 220);
  //playlistbarlayout->setSpacing(0);
  playlistTableWidget = new QTableWidget(trimsize, 1);
  playlistTableWidget->setShowGrid(false);
  playlistTableWidget->horizontalHeader()->setHighlightSections(false);
  playlistTableWidget->horizontalHeader()->setSortIndicatorShown(false);
  playlistTableWidget->setWordWrap(false);
  playlistTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  playlistTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  playlistTableWidget->setColumnWidth(0, 203);
  playlistTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  playlistTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  playlistTableWidget->verticalHeader()->hide();
  playlistTableWidget->setFrameStyle(QFrame::Plain | QFrame::Plain);

  playlistTableWidget->setLineWidth(0);
  playlistTableWidget->setMidLineWidth(0);
  playlistTableWidget->setFocusPolicy(Qt::NoFocus);
  playlistTableWidget->horizontalHeader()->hide();

  QObject::connect(playlistTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(DoubleClicked(int,int)));

  playlistTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(playlistTableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));

  playlistTableWidget->setObjectName("sideBarPlaylistTableWidget");
  playlistTableWidget->setStyleSheet(util.getStylesheet());
  return playlistTableWidget;

}

void SidebarController::ShowContextMenu(const QPoint& pos)
{
  Playlist pass(db, (displayTableWidget->itemAt(pos)->text()));
  bool type = typearray[displayTableWidget->rowAt((mapToGlobal(pos).y()))];
  if(type == true)
    smartmenu->playlistrightclicked(&pass, &libpass);
  else
    normalmenu->playlistrightclicked(&pass, &libpass);
  QString sizestring = db.getSetting("windowSize");
  if( sizestring=="" )
    updateplaylistbar( 7 );
  else
  {
    double size = sizestring.split("|").at(1).toDouble();
    updateplaylistbar( (int)(size/89.25) );
  }
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
  if(row != 2 || row != 3)
    qDebug() << test;

}




