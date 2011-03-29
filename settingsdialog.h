#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "sharingtab.h"
#include "database.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(Database &datab, QWidget *parent=0);
signals:
    void accept();
private slots:
    void ok();
private:
    QDialog sdialog;
    SharingTab* stab;
    Database &db;
};

#endif // SETTINGSDIALOG_H
