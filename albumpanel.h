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
    //make the album box in the panel
    QWidget *makeAlbumArt();
private:
    //store the layout
    QGridLayout* grid;
    //store the path of the executable
    QString expath;
    Utilities& util;
signals:

public slots:

};

#endif // ALBUMPANEL_H
