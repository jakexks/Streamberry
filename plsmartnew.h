#ifndef PLSMARTNEW_H
#define PLSMARTNEW_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;
class Database;


class PLSmartNew : public QDialog
{
   Q_OBJECT
public:
    PLSmartNew(Utilities* util, Database& datab);
private slots:
    void btnClicked();
private:
    QLineEdit* textbox1;
    QLineEdit* textbox2;
    QDialog* dialog;
    Database& db;

};

#endif // PLSMARTNEW_H
