#ifndef SHARINGTAB_H
#define SHARINGTAB_H

#include <QtGui>

class SharingTab : public QWidget
{
    Q_OBJECT
public:
    SharingTab(QStringList sfiles, QWidget *parent = 0);
    ~SharingTab();
    QString getSelectedFilepaths();
private:
    QTreeView *tree;
    QFileSystemModel *model;
    QStringList selectedfiles;
    void expandRows(QString filepath);
};

#endif // SHARINGTAB_H
