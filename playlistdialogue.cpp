#include "playlistdialogue.h"
#include <QtGui>
#include <QDialog>
#include "playlist.h"
#include <utilities.h>


PLRenameDialog::PLRenameDialog(Playlist* playlist, Utilities* util)
{
  currentplaylist = playlist;
  dialog = new QDialog();
  QGridLayout layout(dialog);

  dialog->setModal(true);
  dialog->setSizeGripEnabled(false);
  QPushButton* accept = new QPushButton("Ok");
  QObject::connect(accept, SIGNAL(clicked()), this, SLOT(btnClicked()));

  QLabel* text = new QLabel("Please Enter New Playlist Name:");
  QString currenttitle = currentplaylist->getPlaylistName();
  textbox = new QLineEdit(currenttitle);
  QObject::connect(textbox, SIGNAL(editingFinished()), this, SLOT(btnClicked()));

  this->setFocusPolicy(Qt::StrongFocus);

  QString iconpath = util->getExecutePath();
  iconpath += "images/icon.ico";
  dialog->setWindowIcon((QIcon(iconpath)));

  layout.addWidget(text, 0, 0, Qt::AlignHCenter);
  layout.addWidget(textbox, 1, 0, Qt::AlignHCenter);
  layout.setColumnMinimumWidth(0, 180);
  layout.addWidget(accept, 2, 0, Qt::AlignHCenter);

  dialog->exec();
}

void PLRenameDialog::btnClicked()
{
  QString text = textbox->text();
  currentplaylist->deletePlaylist();
  currentplaylist->setPlaylistName(text);
  currentplaylist->SavePlaylist();
  dialog->accept();
}
