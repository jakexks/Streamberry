#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"
#include "database.h"
#include <QThread>
//#include "streamfile.h"
#include <QDebug>
#include "database.h"
#include "sbexception.h"
#include "filescan.h"
#include "playlist.h"
#include "filemeta.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utilities util(argv[0]);
    Database db = Database();
    Database gb = Database();
    MainWindow w(util, gb);
    w.show();

    //StreamFile stream;
    //stream.addStream("/Users/Robbie/Music/Albums/Biffy Clyro - Only Revolutions/Biffy Clyro - Many Of Horror.mp3", "test", "127.0.0.1");
    //qDebug() << stream.getStreamLength("test");

    db.initialise();
      Filescan scanner(db);
      try
      {
          db.setFolders("C:/Users/Jim/Music");
          db.setUniqueID( (QString)"98" );
          //scanner.build_new_clean();
          //importLib itunes("");//file name here.
          //QList<QString> allFiles = itunes.libFileList();
          //PLAYLIST TEST
          Playlist test1 = Playlist(db);
          test1.setPlaylistName("test1");
          test1.setPlaylistType("nothing");
          test1.setFilter("");
          test1.addTrack(1, "98");
          test1.addTrack(12, "98");
          test1.addTrack(13, "98");
          test1.addTrack(16, "98");
          test1.addTrack(10, "98");
          test1.SavePlaylist();
          test1.addTrack(56, "98");
          test1.addTrack(120, "98");
          test1.SavePlaylist();
          test1.removeTrack(3);
          test1.removeTrack(56, "98");
          test1.SavePlaylist();
          Playlist test2 = Playlist(db);
          test2.setPlaylistName("test2");
          test2.setPlaylistType("nothing");
          test2.setFilter("");
          test2.addTrack(145, "98");
          test2.addTrack(146, "98");
          test2.addTrack(147, "98");
          test2.SavePlaylist();
          test1.deletePlaylist();
          Playlist test3 = Playlist(db, "test2");
          QList<QSqlRecord> tracks = test3.getAllTracks();
          int i=0;
          for(i=0; i<(tracks.size()); i++)
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
