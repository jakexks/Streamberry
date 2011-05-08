#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtGui>
#include "database.h"
#include "filescan.h"

//The settings dialogue window
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(Database &datab, Filescan &fscan, QWidget *parent=0);
    ~SettingsDialog();
public slots:
    void scanFinished();
signals:
    void startScan();
private slots:
    void ok();
    void scanStarting();
private:
    QDialog sdialog;
    QString getSelectedFilepaths();
    QTreeView *tree;
    QFileSystemModel *model;
    QLineEdit *nickedit;
    QStackedWidget *sw;
    Database &db;
    Filescan &fs;
    void expandRows(QString filepath);
};

#endif // SETTINGSDIALOG_H
