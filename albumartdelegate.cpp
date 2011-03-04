#include "albumartdelegate.h"
#include "albumpanel.h"
#include <QtGui>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDoubleValidator>
#include <QDebug>

AlbumArtDelegate::AlbumArtDelegate(Utilities &utilities, QObject *parent) : util(utilities), QStyledItemDelegate(parent)
{
}

QWidget* AlbumArtDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return NULL;
}

void AlbumArtDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex &index) const
{
    //    QLineEdit* line = static_cast<QLineEdit*>(editor);
    //    QString value = line->text();
    //    model->setData(index,value);
}

void AlbumArtDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QWidget *editor = new QWidget();
    editor->setFixedSize(option.rect.size());
    editor->setObjectName("albumArt");
    editor->setStyleSheet(util.getStylesheet());
    AlbumPanel *p = new AlbumPanel("", editor);
    QGridLayout grid(editor);
    grid.setMargin(0);
    grid.setContentsMargins(0,0,0,0);
    grid.addWidget(p, 0, 0);
    painter->translate(option.rect.x(), option.rect.y());
    editor->render(painter);
    painter->restore();
}
