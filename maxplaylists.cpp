#include "maxplaylists.h"
#include <QtGui>
#include <QDialog>
#include "playlist.h"
#include <utilities.h>
#include <database.h>


MaxPlaylists::MaxPlaylists(Utilities* util, Database& datab)
{
  dialog = new QDialog();
  QGridLayout layout(dialog);

  dialog->setModal(true);
  dialog->setSizeGripEnabled(false);


  QLabel* text1 = new QLabel("Sorry in this demo version of Streamberry, you cannot have more than 10 playlists");
  QPushButton* accept = new QPushButton("Ok");

  QObject::connect(accept, SIGNAL(clicked()), this, SLOT(btnClicked()));


  QString iconpath = util->getExecutePath();
  iconpath += "images/icon.ico";
  dialog->setWindowIcon((QIcon(iconpath)));

  layout.addWidget(text1, 0, 0, Qt::AlignHCenter);
  layout.addWidget(accept, 3, 0, Qt::AlignHCenter);

  layout.setColumnMinimumWidth(0, 180);
  dialog->exec();
}

void MaxPlaylists::btnClicked()
{
  dialog->accept();
}
