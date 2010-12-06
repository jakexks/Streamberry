#include <QtGui/QApplication>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include "mainwindow.h"
#include "database.h"
#include "sbexception.h"
#include "filescan.h"
#include "beaconsender.h"
#include "beaconreceiver.h"

using namespace std;


int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   beaconsender b;
   beaconreceiver r;
   b.start();
   r.start();
   Database db = Database();
   Filescan scanner(db);
   try
   {

       db.setFolders("/Users/Robbie/Music/Albums/;");
       //scanner.build_new();



   }
   catch(SBException e)
   {
       qDebug() << e.getException();
   }
//    //testing storing settings
//    try
//    {
//        db.storeSetting("Test1", "tvalue1");
//        db.storeSetting("Test2", "tvalue2");
//        db.storeSetting("Test1", "tvalue3");

//        qDebug() << "Test1 = " << db.getSetting("Test1");
//        //should return null
//        qDebug() << "Not in database = " << db.getSetting("notthere");

//        //testing setting and getting folders
//        db.setFolders("folder1;folder2");
//        db.setFolders("folder3;folder4;folder5;");

//        QStringList strlst = db.getFolders(0);

//        qDebug() << "Folders in database. size: " << strlst.size() << ".\n";
//        for(int i = 0; i<strlst.size(); i++)
//        {
//            qDebug() << strlst.value(i) << "\n";
//        }

//        qDebug() << "Tracked folder count: " << db.rowCount("TrackedFolders");

//        db.addFile("/test", "a", "a", "artist", "album", "the title", "the genre", "5", "1992", "367", "123", "mp3", "LibLocal");

//        QList<QSqlRecord> files = db.searchDb(0, "");

//        qDebug() << "First file in database: " << files.value(0).value(0).toString();

//        qDebug() << "Changes: " << db.changesSinceTime(1, "TESTID") << endl;
//    }
//    catch(SBException e)
//    {
//        qDebug() << e.getException();
//    }
/*#include <qthread.h>
#include "threadtest.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include <QDebug>
#include <mainwindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    beaconsender b;
    b.start();
    beaconreceiver r;
    r.start();*/
    return a.exec();
}
