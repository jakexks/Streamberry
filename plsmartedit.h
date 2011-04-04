#ifndef PLSMARTEDIT_H
#define PLSMARTEDIT_H
#include <QDialog>
#include <QObject>
#include <QWidget>
class Utilities;
class Playlist;
class QLineEdit;


class PLSmartEdit : public QDialog
{
   Q_OBJECT
public:
    PLSmartEdit(Playlist* playlist, Utilities* util);
private slots:
    void btnClicked();
private:
    Playlist* currentplaylist;
    QLineEdit* textbox;
    QDialog* dialog;

};

#endif // PLSMARTEDIT_H
