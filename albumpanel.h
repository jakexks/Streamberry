#ifndef ALBUMPANEL_H
#define ALBUMPANEL_H

#include <QWidget>
#include <QGridLayout>


class AlbumPanel : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumPanel(QString expath, QWidget *parent = 0);
    QWidget *makeAlbumArt();
private:
    QGridLayout* grid;
    QString expath;
signals:

public slots:

};

#endif // ALBUMPANEL_H
