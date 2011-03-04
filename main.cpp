#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>
#include "streamfile.h"
#include <QDebug>
#include "database.h"
#include "sbexception.h"
#include "filescan.h"
#include "playlist.h"
#include "filemeta.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include "player.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Utilities util(a.applicationDirPath());

  Database db;
  Player player;

    BeaconSender *bs = new BeaconSender(db);
    QThread *bsthread = new QThread(&a);
    bs->moveToThread(bsthread);
    bsthread->start();

    BeaconReceiver *br = new BeaconReceiver(db);
    QThread *brthread = new QThread(&a);
    br->moveToThread(brthread);
    brthread->start();

    QObject::connect(&a, SIGNAL(aboutToQuit()), bs, SLOT(sendOfflineBeacon()));

  MainWindow w(util, db, player);

  w.show();

  //StreamFile stream;
  //stream.addStream("/Users/Robbie/Music/Albums/Biffy Clyro - Only Revolutions/Biffy Clyro - Many Of Horror.mp3", "test", "127.0.0.1");
  //qDebug() << stream.getStreamLength("test");

  try
  {

    db.setFolders("/streamberrytest/");
//    db.setFolders("C:\\Users\\Jim\\Music");
//    db.setFolders("C:\\Users\\Jim\\Music\\TEST");
//    Filescan fs(db);
//    QThread fsthread;
//    fs.moveToThread(&fsthread);
//    fs.build_new_clean();

//    importLib itunes("");//file name here.
//    QList<QString> allFiles = itunes.libFileList();
    //PLAYLIST TEST

    Playlist test1 = Playlist(db);
    test1.setPlaylistName("test1");
    test1.setPlaylistType(0);
    test1.setFilter("");
    test1.addTrack(1, "Local");
    test1.addTrack(12, "Local");
    test1.addTrack(13, "Local");
    test1.addTrack(16, "Local");
    test1.addTrack(10, "Local");
    test1.SavePlaylist();
    test1.addTrack(56, "Local");
    test1.addTrack(120, "Local");
    test1.SavePlaylist();
    test1.removeTrack(3);
    test1.removeTrack(56, "Local");
    test1.SavePlaylist();
    Playlist test2 = Playlist(db);
    test2.setPlaylistName("test2");
    test2.setPlaylistType(0);
    test2.setFilter("");
    test2.addTrack(145, "Local");
    test2.addTrack(146, "Local");
    test2.addTrack(147, "Local");
    test2.SavePlaylist();
    test1.deletePlaylist();
    Playlist test3 = Playlist(db, "test2");
    QList<QSqlRecord>* tracks = test3.getAllTracks();
    int i=0;
    for(i=0; i<(tracks->size()); i++)
    {
      //qDebug() << tracks.at(i).value(2).toString();
    }
    qDebug() << "Tests completed";
  }
  catch(SBException e)
  {
    qDebug() << e.getException();
  }

  return a.exec();


}
