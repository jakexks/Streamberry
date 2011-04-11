#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

#include <QWizard>
#include <QtGui>

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
    FirstRunWizard(QWidget *parent = 0);
    void accept();
private:
    SharingPage *sharingpage;
};


class NickPage : public QWizardPage
{
    Q_OBJECT
public:
    NickPage(QWidget *parent = 0);

};

#endif // FIRSTRUNWIZARD_H
