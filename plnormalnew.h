#ifndef PLNORMALNEW_H
#define PLNORMALNEW_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;
class Database;


class PLNormalNew : public QDialog
{
   Q_OBJECT
public:
    PLNormalNew(Utilities* util, Database& datab);
private slots:
    void btnClicked();
private:
    //Playlist* currentplaylist;
    QLineEdit* textbox;
    QDialog* dialog;
    Database& db;

};

#endif // PLNORMALNEW_H
