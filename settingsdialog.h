#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "sharingtab.h"
#include "database.h"
#include "misctab.h"
#include "filescan.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(Database &datab, Filescan &fscan, QWidget *parent=0);
signals:
    void accept();
private slots:
    void ok();
private:
    QDialog sdialog;
    SharingTab* stab;
    MiscTab* mtab;
    Database &db;
    Filescan &fs;
};

#endif // SETTINGSDIALOG_H
