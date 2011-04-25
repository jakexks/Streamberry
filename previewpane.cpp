#include "previewpane.h"
#include <QtGui>

PreviewPane::PreviewPane(Utilities &utilities, Database& datab, LibraryController* lib)
{
  util = &utilities;
  db = &datab;
  libcont = lib;
  mainwidget = new QWidget();
  mainwidget->setObjectName("sideBarPreviewPane");
  mainwidget->setStyleSheet(util->getStylesheet());
  QGridLayout* previewPaneLayout = new QGridLayout(mainwidget);
  previewPaneLayout->setMargin(0);
  previewPaneLayout->setSpacing(0);
  window = new QStackedWidget(mainwidget);
  window->setMinimumSize(139, 139);
  window->setMaximumSize(139, 139);
  window->setObjectName("sideBarPreviewPic");
  window->setStyleSheet(util->getStylesheet());
  previewPaneLayout->addWidget(window, 0,0, 2, 1,  Qt::AlignHCenter);

  QWidget* default1 = makeDefault();
  QWidget* playlist = makePlaylist();
  QWidget* art = makeArt();

  window->addWidget(default1);
  window->addWidget(playlist);
  window->addWidget(art);

  state =0;
}

QWidget* PreviewPane::makeDefault()
{
  QWidget* pane = new QWidget(window);
  return pane;
}

QWidget* PreviewPane::makePlaylist()
{
  QWidget* pane = new QWidget(window);
  return pane;
}

QWidget* PreviewPane::makeArt()
{
  QWidget* pane = new QWidget(window);
  return pane;
}

void PreviewPane::rolloverDefault()
{
  state =0;
  window->setCurrentIndex(0);
}

void PreviewPane::rolloverPlaylist(QString name)
{
  updatePreview(name);
  state = 1;
  window->setCurrentIndex(1);
}

void PreviewPane::displayAlbumArt(QImage pic)
{
  updateArt(pic);
  state = 2;
  window->setCurrentIndex(2);
}


void PreviewPane::updateArt(QImage pic)
{

}

void PreviewPane::updatePreview(QString name)
{

}











PreviewPane::~PreviewPane()
{
  delete mainwidget;
}

QWidget* PreviewPane::getwidget()
{
  return mainwidget;
}
