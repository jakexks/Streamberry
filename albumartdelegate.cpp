#include "albumartdelegate.h"
#include "albumpanel.h"
#include <QtGui>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDoubleValidator>
#include <QDebug>

AlbumArtDelegate::AlbumArtDelegate(Utilities &utilities, QObject *parent) : QStyledItemDelegate(parent), util(utilities)
{
    editor = new QWidget();

    editor->setObjectName("albumArt");
    editor->setStyleSheet(util.getStylesheet());
}

QWidget* AlbumArtDelegate::createEditor(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const
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
    editor->setFixedSize(option.rect.size());
    AlbumPanel *p = new AlbumPanel(util,"", editor);
    QGridLayout grid(editor);
    grid.setMargin(0);
    grid.setContentsMargins(0,0,0,0);
    grid.addWidget(p, 0, 0);
    painter->translate(option.rect.x(), option.rect.y());
    editor->render(painter);
    painter->restore();
    delete p;
}
