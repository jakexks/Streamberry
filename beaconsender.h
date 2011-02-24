#ifndef BEACONSENDER_H
#define BEACONSENDER_H
#include "database.h"

class BeaconSender : public QObject
{
    Q_OBJECT
public:
    BeaconSender(Database &datab);
public slots:
    void sendOfflineBeacon();
private slots:
    void send(bool online = true);
protected:
private:
    Database& db;
    QTimer *timer;
    QString myip;
};

#endif // BEACONSENDER_H
