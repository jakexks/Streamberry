#ifndef ALBUMARTDELEGATE_H
#define ALBUMARTDELEGATE_H

#include <QStyledItemDelegate>
#include <QtGui>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDoubleValidator>
#include "utilities.h"

//Creates album panels when they are needed by the table
class AlbumArtDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    AlbumArtDelegate(Utilities &utilities, QObject* parent = 0);
    //not used but must be overridden so it is implemented
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //give the panel some data to show
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    //redraw the panel
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    Utilities& util;
};

#endif // ALBUMARTDELEGATE_H
