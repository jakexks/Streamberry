#include "librarycontroller.h"
#include "sbexception.h"
#include "utilities.h"
#include "albumartdelegate.h"
#include "database.h"
#include <QtGui>
#include <QString>
#include <QGridLayout>
#include <QSqlField>
#include <QDebug>
#include <QFontMetrics>

#define DEFAULT_WIDTH 170
#define DEFAULT_ARTPANEL_WIDTH 160

LibraryController::LibraryController(Utilities& utilities, Database& datab)
    : util(utilities), db(datab)
{
    curheaders = NULL;

    QList<QString> headers;
    headers.append("Title");
    headers.append("Artist");
    headers.append("Album");
    setHeaders(headers, 2);
    widget = new QWidget();
    container = new QGridLayout(widget);
    curview = NULL;
    currentdata = NULL;
    paneldelegate = new AlbumArtDelegate();
    makeWidget();

    QObject::connect(tablewidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
}

QWidget* LibraryController::getWidget()
{
    return widget;
}

void LibraryController::makeWidget()
{
    if(curview!=NULL)
    {
        container->removeWidget(curview);
    }

    curview = new QWidget();
    curview->setObjectName("library");

    QGridLayout *temp = new QGridLayout(curview);
    temp->setMargin(0);

    tablewidget = new QTableWidget();
    tablewidget->setObjectName("libraryTableWidget");
    //tablewidget->setGridStyle(Qt::NoPen);
    tablewidget->setShowGrid(false);
    tablewidget->horizontalHeader()->setHighlightSections(false);
    tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //tablewidget->horizontalHeader()->setStretchLastSection(true);
    tablewidget->horizontalHeader()->setSortIndicatorShown(true);
    tablewidget->setWordWrap(false);
    temp->addWidget(tablewidget);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    tablewidget->setItemPrototype(item);

    addHeaders();

    tablewidget->setItemDelegateForColumn(0, paneldelegate);

    QObject::connect(tablewidget, SIGNAL(itemSelectionChanged(void)), this, SLOT(deselectFirst(void)));
    QObject::connect(tablewidget->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortIndicatorChanged(int,Qt::SortOrder)));

    container->addWidget(curview, 0, 0);
}

void LibraryController::addHeaders()
{
    QTableWidgetItem* header;
    QFont font;
    font.setStyleHint(QFont::System, QFont::PreferAntialias);
    font.setPointSize(11);

    tablewidget->setColumnCount(headercount + 2);
    sortorder = Qt::AscendingOrder;

    tablewidget->verticalHeader()->setVisible(false);
    header = new QTableWidgetItem("");
    tablewidget->setHorizontalHeaderItem(0, header);

    QString setting;

    if((setting = db.getSetting("headerAlbumPanel"))!=NULL)
    {
        tablewidget->setColumnWidth(0, setting.toInt());
    }
    else
    {
        tablewidget->setColumnWidth(0, DEFAULT_ARTPANEL_WIDTH);
    }

    header = new QTableWidgetItem("");
    tablewidget->setHorizontalHeaderItem(1, header);
    tablewidget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    QFontMetrics fontm = QFontMetrics(font);
    tablewidget->verticalHeader()->setDefaultSectionSize(fontm.lineSpacing()+fontm.height());

    for(int i = 0; i<headercount; i++)
    {
        header = new QTableWidgetItem(*curheaders[i]);
        QString settingname = "header";
        settingname += *curheaders[i];

        if((setting = db.getSetting(settingname))!=NULL)
        {
            tablewidget->setColumnWidth(i+2, setting.toInt());
        }
        else
        {
            tablewidget->setColumnWidth(i+2, DEFAULT_WIDTH);
        }

        tablewidget->setHorizontalHeaderItem(i+2, header);
    }

    tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, Qt::AscendingOrder);
    tablewidget->horizontalHeader()->setMovable(true);
}

void LibraryController::fillData(QList<QSqlRecord> *values)
{
    if(currentdata!=NULL)
    {
        delete currentdata;
    }

    currentdata = values;
    tablewidget->setRowCount(0);
    tablewidget->setRowCount(values->length());

    int length = values->length();
    QString lastalbum;
    int albumstart = -1;
    QColor background("#DCE5EA");
    QColor background2("#FEFEFE");
    QFont font;
    font.setStyleHint(QFont::System, QFont::PreferAntialias);
    font.setPointSize(11);

    for(int i = 0; i<length; i++)
    {
        QSqlRecord currecord = values->at(i);
        QString curalbum = currecord.field(*curheaders[sortcolumn]).value().toString();
        //if the album has changed, start a new left column
        if(curalbum!=lastalbum)
        {
            if(albumstart>-1)
            {
                tablewidget->setSpan(albumstart, 0, (i-albumstart), 1);
            }

            lastalbum = curalbum;
            albumstart = i;
        }

        QTableWidgetItem* item = new QTableWidgetItem(currecord.field("Track").value().toString());
        item->setFont(font);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        if(i%2==1)
            item->setBackgroundColor(background);
        else
            item->setBackgroundColor(background2);

        tablewidget->setItem(i, 1, item);

        //put track in the view
        for(int j = 0; j < headercount; j++)
        {
            item = new QTableWidgetItem(currecord.field(*curheaders[j]).value().toString());
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            item->setFont(font);

            if(i%2==1)
                item->setBackgroundColor(background);
            else
                item->setBackgroundColor(background2);

            tablewidget->setItem(i, j+2, item);
        }
    }

    if(length>0)
    {
        tablewidget->setSpan(albumstart, 0, (length-albumstart), 1);
    }

    tablewidget->resizeColumnToContents(1);
}

void LibraryController::setHeaders(QList<QString>& headers, int sortcol)
{
    int headerlen = headers.length();
    if(sortcol<1 || sortcol>=headerlen || headerlen<1)
    {
        throw new SBException(GENERIC, "No headers or invalid sort column.");
    }

    if(curheaders!=NULL)
    {
        //free the old header strings
        for(int i = 0; i<headercount; i++)
        {
            delete curheaders[i];
        }

        delete curheaders;
    }

    //make new ones
    headercount = headerlen;
    sortcolumn = sortcol;
    curheaders = new QString*[headercount];

    for(int i = 0; i<headercount; i++)
    {
        curheaders[i] = new QString(headers.at(i));
    }
}

void LibraryController::deselectFirst()
{
    QList<QTableWidgetSelectionRange> selections = tablewidget->selectedRanges();

    foreach(QTableWidgetSelectionRange selection, selections)
    {
        tablewidget->setRangeSelected(QTableWidgetSelectionRange(selection.topRow(), 0, selection.bottomRow(), 0), false);
    }
}

void LibraryController::sortIndicatorChanged(int index, Qt::SortOrder order)
{
    QList<QString> *sortcols = new QList<QString>();
    QList<QString> *orders = new QList<QString>();
    QString orderstr = (order==Qt::AscendingOrder) ? "ASC" : "DESC";

    if(index<2)
    {
        tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
    }
    else if(index-2<headercount && !(index==sortcolumn+2 && order==sortorder))
    {
        if(*curheaders[index-2] == "Artist")
        {
            sortcols->append("Artist");
            sortcols->append("Album");
            sortcols->append("Track");
            orders->append(orderstr);
            orders->append("ASC");
            orders->append("ASC");
        }
        else if(*curheaders[index-2] == "Album")
        {
            sortcols->append("Album");
            sortcols->append("Track");
            orders->append(orderstr);
            orders->append("ASC");
        }
        else if(*curheaders[index-2] == "Genre")
        {
            sortcols->append("Genre");
            sortcols->append("Artist");
            sortcols->append("Album");
            sortcols->append("Track");
            orders->append(orderstr);
            orders->append("ASC");
            orders->append("ASC");
            orders->append("ASC");
        }
        else
        {
            sortcols->append(*curheaders[index-2]);
            orders->append(orderstr);
        }

        sortcolumn = index - 2;
        sortorder = order;
        emit needNewLibrary(sortcols, orders);
    }
}

void LibraryController::sectionResized(int logicalindex, int oldsize, int newsize)
{
    try
    {
        if(logicalindex==0)
        {
            db.storeSetting("headerAlbumPanel", QString::number(newsize));
        }
        else if(logicalindex>1)
        {
            QString temp = "header";
            temp += *curheaders[logicalindex-2];
            db.storeSetting(temp, QString::number(newsize));
        }
    }
    catch(SBException e)
    {
        //qDebug() << e.getException();
    }
}

LibraryController::~LibraryController()
{
    delete paneldelegate;
}