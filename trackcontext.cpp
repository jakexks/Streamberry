#include "trackcontext.h"
#include <QMenu>
#include "utilities.h"
#include "librarycontroller.h"
#include <QtGui>
#include "database.h"
#include "addto.h"

TrackContext::TrackContext(Database* datab): db(datab)
{
  setup = 0;

  addto = new AddTo(db);
  addto->setTitle("Add To...");
  menuitems[0] = this->addAction("Play");
  menuitems[1] = this->addSeparator();
  menuitems[2] = this->addAction("Delete from my Library");
  menuitems[3] = this->addAction("Toggle Hidden from Other Users");
  menuitems[4] = this->addMenu(addto);

  QObject::connect(menuitems[0], SIGNAL(triggered()), this, SLOT(Play()));
  QObject::connect(menuitems[2], SIGNAL(triggered()), this, SLOT(Delete()));
  QObject::connect(menuitems[3], SIGNAL(triggered()), this, SLOT(Hide()));
}

void TrackContext::trackRightClicked(QList<QString> ids, QList<QString> uniqueIDs, LibraryController* passlib)
{
  qDebug() << ids;
  CurrentIDs = ids;
  CurrentUniqueIDs = uniqueIDs;
  addto->update(ids, uniqueIDs);

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

