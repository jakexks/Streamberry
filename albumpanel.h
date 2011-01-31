#ifndef ALBUMPANEL_H
#define ALBUMPANEL_H

#include <QWidget>
#include <QGridLayout>


class AlbumPanel : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumPanel(QWidget *parent = 0);
    QWidget *makeAlbumArt();
private:
    QGridLayout* grid;
signals:

public slots:

};

#endif // ALBUMPANEL_H
