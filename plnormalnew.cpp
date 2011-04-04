#include "plnormalnew.h"
#include <QtGui>
#include <QDialog>
#include "playlist.h"
#include <utilities.h>
#include <database.h>


PLNormalNew::PLNormalNew(Utilities* util, Database& datab): db(datab)
{
  dialog = new QDialog();
  QGridLayout layout(dialog);

  dialog->setModal(true);
  dialog->setSizeGripEnabled(false);


  QLabel* text1 = new QLabel("Please Enter New Playlist Name:");
  textbox = new QLineEdit("");
  QLabel* text2 = new QLabel("Add files to this playlist by dragging and dropping from the library to the playlist pane");
  QPushButton* accept = new QPushButton("Ok");

  QObject::connect(accept, SIGNAL(clicked()), this, SLOT(btnClicked()));
  QObject::connect(textbox, SIGNAL(editingFinished()), this, SLOT(btnClicked()));

  this->setFocusPolicy(Qt::StrongFocus);
  QString iconpath = util->getExecutePath();
  iconpath += "images/icon.ico";
  dialog->setWindowIcon((QIcon(iconpath)));

  layout.addWidget(text1, 0, 0, Qt::AlignHCenter);
  layout.addWidget(textbox, 1, 0, Qt::AlignHCenter);
  layout.addWidget(text2, 2, 0, Qt::AlignHCenter);
  layout.addWidget(accept, 3, 0, Qt::AlignHCenter);
  layout.setColumnMinimumWidth(0, 180);

  dialog->exec();
}

void PLNormalNew::btnClicked()
{
  QString newname = textbox->text();
  Playlist newplaylist(db, newname);
  newplaylist.setPlaylistName(newname);
  newplaylist.SavePlaylist();
  dialog->accept();
}
