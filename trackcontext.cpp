#include "trackcontext.h"
#include <QMenu>
#include "utilities.h"
#include "librarycontroller.h"
#include <QtGui>
#include "database.h"

TrackContext::TrackContext()
{
  setup = 0;
  QAction* menuitems[4];
  menuitems[0] = this->addAction("Play");
  menuitems[1] = this->addSeparator();
  menuitems[2] = this->addAction("Delete from my Library");
  menuitems[3] = this->addAction("Toggle Hidden from Other Users");

  QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(Play()));
  QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(Delete()));
  QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(Hide()));
}

void TrackContext::trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib, Database* datab)
{
  CurrentIDs = ids;
  qDebug() << CurrentIDs;
  db = datab;
  CurrentUniqueIDs = uniqueIDs;
  if(setup == 0)
  {
    LibCont = passlib;
    QObject::connect(this, SIGNAL(playthis(QList<QSqlRecord>*)), LibCont, SLOT(displaythis(QList<QSqlRecord>*)));
    setup = 1;
  }
  this->exec(QCursor::pos());

}


void TrackContext::Play()
{
  qDebug() << "Play Now!";
}

void TrackContext::Delete()
{
  qDebug() << "Delete Track from Library";
}

void TrackContext::Hide()
{
  int i =0;
  for(i=0; i<CurrentIDs.size(); i++)
  {
    db->togglehidden(CurrentIDs.at(i), CurrentUniqueIDs.at(i));
  }

}
