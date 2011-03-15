#ifndef ALBUMPANEL_H
#define ALBUMPANEL_H

#include <QWidget>
#include <QGridLayout>
#include "utilities.h"


class AlbumPanel : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumPanel(Utilities& utilities, QString expath, QWidget *parent = 0);
    QWidget *makeAlbumArt();
private:
    QGridLayout* grid;
    QString expath;
    Utilities& util;
signals:

public slots:

};

#endif // ALBUMPANEL_H
