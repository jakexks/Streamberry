#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

#include <QWizard>
#include <QtGui>
#include "database.h"
#include "filescan.h"

class SharingPage: public QWizardPage
{
    Q_OBJECT
public:
    SharingPage(QWidget *parent = 0);
    QString getSelectedFiles();
private:
    QTreeView *tree;
    QFileSystemModel *model;
    void expandRows(QString);
};

class FirstRunWizard : public QWizard
{
    Q_OBJECT
public:
    FirstRunWizard(Database &datab, Filescan &fscan, QWidget *parent = 0);
    void accept();
private:
    SharingPage *sharingpage;
    Database &db;
    Filescan &fs;
};


class NickPage : public QWizardPage
{
    Q_OBJECT
public:
    NickPage(QWidget *parent = 0);

};

#endif // FIRSTRUNWIZARD_H
