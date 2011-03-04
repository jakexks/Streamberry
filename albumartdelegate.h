#ifndef ALBUMARTDELEGATE_H
#define ALBUMARTDELEGATE_H

#include <QStyledItemDelegate>
#include <QtGui>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDoubleValidator>
#include "utilities.h"

class AlbumArtDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    AlbumArtDelegate(Utilities &utilities, QObject* parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    Utilities& util;
};

#endif // ALBUMARTDELEGATE_H
