#include <qthread.h>
#include "threadtest.h"
#include "beaconsender.h"
#include "beaconreceiver.h"
#include "networking.h"

class I : public QThread
{
public:
        static void sleep(unsigned long secs) {
                QThread::sleep(secs);
        }
        static void msleep(unsigned long msecs) {
                QThread::msleep(msecs);
        }
        static void usleep(unsigned long usecs) {
                QThread::usleep(usecs);
        }
};


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();
    qDebug() << "Test main";
    networking n;
    n.getuniqid();
    beaconsender b;
    b.start();
    beaconreceiver r;
    r.start();
    while (true)
        I::sleep(5);
    return 0;
}
