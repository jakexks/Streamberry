#ifndef ALBUMARTDELEGATE_H
#define ALBUMARTDELEGATE_H

#include <QStyledItemDelegate>
#include <QtGui>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDoubleValidator>

class AlbumArtDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    AlbumArtDelegate(QObject* parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ALBUMARTDELEGATE_H
