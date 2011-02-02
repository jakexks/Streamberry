#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include <QThread>

class BeaconSender : public QThread
{
    Q_OBJECT
public:
    BeaconSender();
public slots:
    void sendOfflineBeacon();
protected:
    void run();

};

#endif // BEACONSENDER_H
