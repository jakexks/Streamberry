#ifndef MISCTAB_H
#define MISCTAB_H

#include <QtGui>
#include "filescan.h"

class MiscTab : public QWidget
{
    Q_OBJECT
public:
    MiscTab(QString currentnick, Filescan &fscan, QWidget *parent = 0);
    QString getNick();
public slots:
    void scanFinished();
private slots:
    void scanStarted();
private:
    QLineEdit *nickedit;
    QString cnick;
    Filescan &fs;
    QStackedWidget *sw;
};

#endif // MISCTAB_H
