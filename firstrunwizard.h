#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

#include <QWizard>
#include <QtGui>
#include "database.h"

class SharingPage: public QWizardPage
{
    Q_OBJECT
public:
    SharingPage(QWidget *parent = 0);
    QString getSelectedFiles();
private:
    QTreeView *tree;
    QFileSystemModel *model;
};

class FirstRunWizard : public QWizard
{
    Q_OBJECT
public:
    FirstRunWizard(Database &datab, QWidget *parent = 0);
    void accept();
private:
    SharingPage *sharingpage;
    Database &db;
};


class NickPage : public QWizardPage
{
    Q_OBJECT
public:
    NickPage(QWidget *parent = 0);

};

#endif // FIRSTRUNWIZARD_H
