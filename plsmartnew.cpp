#include "plsmartnew.h"
#include <QtGui>
#include <QDialog>
#include "playlist.h"
#include <utilities.h>
#include <database.h>


PLSmartNew::PLSmartNew(Utilities* util, Database& datab): db(datab)
{
  dialog = new QDialog();
  QGridLayout layout(dialog);

  dialog->setModal(true);
  dialog->setSizeGripEnabled(false);


  QLabel* text1 = new QLabel("Please Enter New Playlist Name:");
  textbox1 = new QLineEdit("");
  QLabel* text2 = new QLabel("Please Enter Your Search Terms:");
  textbox2 = new QLineEdit("");
  QPushButton* accept = new QPushButton("Ok");

  QObject::connect(accept, SIGNAL(clicked()), this, SLOT(btnClicked()));
  QObject::connect(textbox2, SIGNAL(editingFinished()), this, SLOT(btnClicked()));

  this->setFocusPolicy(Qt::StrongFocus);
  QString iconpath = util->getExecutePath();
  iconpath += "images/icon.ico";
  dialog->setWindowIcon((QIcon(iconpath)));

  layout.addWidget(text1, 0, 0, Qt::AlignHCenter);
  layout.addWidget(textbox1, 1, 0, Qt::AlignHCenter);
  layout.addWidget(text2, 2, 0, Qt::AlignHCenter);
  layout.addWidget(textbox2, 3, 0, Qt::AlignHCenter);
  layout.addWidget(accept, 4, 0, Qt::AlignHCenter);
  layout.setColumnMinimumWidth(0, 180);

  dialog->exec();
}

void PLSmartNew::btnClicked()
{
  QString newname = textbox1->text();
  QString newfilter = textbox2->text();
  if(newname !="")
  {
    Playlist newplaylist(db, newname);
    newplaylist.setPlaylistType(1);
    newplaylist.setPlaylistName(newname);
    newplaylist.setFilter(newfilter);
    newplaylist.SavePlaylist();
  }
  dialog->accept();
}
