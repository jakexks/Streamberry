#include "librarycontroller.h"
#include "sbexception.h"
#include "utilities.h"
#include "albumartdelegate.h"
#include "database.h"
#include "sbsearchbar.h"
#include <QtGui>
#include <QString>
#include <QGridLayout>
#include <QSqlField>
#include <QDebug>
#include <QFontMetrics>
#include "player.h"
#include <QGradient>

#define DEFAULT_WIDTH 170
#define DEFAULT_ARTPANEL_WIDTH 160

LibraryController::LibraryController(Utilities& utilities, Database& datab, Player& p, SBSearchBar* searchbar)
    : util(utilities), db(datab), player(p)
{
    curheaders = NULL;
    currentlyplaying = -1;
    searchtext = "";
    sortcols = NULL;
    orders = NULL;

    QStringList headers;
    QString headerstr;

    if((headerstr = db.getSetting("TableHeaders"))==NULL)
    {
        headers.append("Title");
        headers.append("Time");
        headers.append("Artist");
        headers.append("Album");
        headers.append("Genre");
        db.storeSetting("TableHeaders", "Title|Time|Artist|Album|Genre");
    } else {
        headers = headerstr.split("|", QString::SkipEmptyParts);
    }

    setHeaders(headers, 3);
    widget = new QWidget();
    container = new QGridLayout(widget);
    container->setMargin(0);
    curview = NULL;
    currentdata = NULL;
    paneldelegate = new AlbumArtDelegate(util);
    makeWidget();

    QObject::connect(tablewidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
    QObject::connect(&player, SIGNAL(getFirstSong()), this, SLOT(playNextFile()));
    QObject::connect(searchbar, SIGNAL(newSearchString(QString)), this, SLOT(setSearchText(QString)));
    QObject::connect(&db, SIGNAL(onlineStatusChange()), this, SLOT(updateLibrary()));
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
    curview->setStyleSheet(util.getStylesheet());
    QGridLayout *temp = new QGridLayout(curview);
    temp->setMargin(0);

    tablewidget = new QTableWidget();
    tablewidget->setObjectName("libraryTableWidget");
    tablewidget->setStyleSheet(util.getStylesheet());
    tablewidget->setShowGrid(false);
    tablewidget->horizontalHeader()->setHighlightSections(false);
    tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    QObject::connect(tablewidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemClicked(int)));

    tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateLibrary();

    container->addWidget(curview, 0, 0);
}

void LibraryController::addHeaders()
{
    QTableWidgetItem* header;
    QFont font;
    font.setStyleHint(QFont::System, QFont::PreferAntialias);
#ifdef Q_WS_WIN
    font.setPointSize(9);
#else
    font.setPixelSize(10);
#endif

    tablewidget->verticalHeader()->setVisible(false);
    tablewidget->setColumnCount(headercount + 2);

    sortorder = Qt::AscendingOrder;
    header = new QTableWidgetItem("");
    tablewidget->setHorizontalHeaderItem(0, header);

    tablewidget->horizontalHeader()->setObjectName("horizontalHeader");
    tablewidget->horizontalHeader()->setStyleSheet(util.getStylesheet());
    tablewidget->horizontalHeader()->setMinimumHeight(20);

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
    //QFontMetrics fontm = QFontMetrics(font);
    //tablewidget->verticalHeader()->setDefaultSectionSize(fontm.lineSpacing()+fontm.height());
    tablewidget->verticalHeader()->setDefaultSectionSize(20);
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
    tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
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
    QColor background(util.getColor(1));
    QColor background2(util.getColor(2));
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

void LibraryController::setHeaders(QStringList& headers, int sortcol)
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

    sortIndicatorChanged(sortcol+2, sortorder);
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
    if(sortcols!=NULL)
    {
        delete sortcols;
    }

    if(orders!=NULL)
    {
        delete orders;
    }

    sortcols = new QList<QString>();
    orders = new QList<QString>();
    QString orderstr = (order==Qt::AscendingOrder) ? "ASC" : "DESC";

    if(index<2)
    {
        //if they click on one of the untitled headers, change it back
        tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
    }
    else if(index-2<headercount)
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
        else if(*curheaders[index-2] == "Time")
        {
            sortcols->append("Length");
            sortcols->append("Track");
            orders->append(orderstr);
            orders->append("ASC");
        }
        else
        {
            sortcols->append(*curheaders[index-2]);
            orders->append(orderstr);
        }

        sortcolumn = index - 2;
        sortorder = order;

        sortLibrary();
    }
}

void LibraryController::sectionResized(int logicalindex, int oldsize, int newsize)
{
    //gets unused parameter warning
    oldsize = 0;

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

void LibraryController::sortLibrary()
{
    updateLibrary();
}

void LibraryController::setSearchText(QString text)
{
    searchtext = text;
    updateLibrary();
}

void LibraryController::updateLibrary()
{
    if(sortcols!=NULL && orders!=NULL && tablewidget!=NULL)
    {
        QList<QSqlRecord> *result = db.searchDb(0, searchtext, *sortcols, *orders);
        fillData(result);
    }
}

void LibraryController::itemClicked(int row)
{
    //Title = x2, Artist = x3, Album = x4
    //QTableWidgetItem *record;

    QSqlRecord record = currentdata->at(row);
    QString filepath = record.field("FilePath").value().toString();
    qDebug() << "Currently playing: " << filepath;
    if(record.field("UniqueID").value() != "local")
    {
        qDebug() << "NOT LOCAL";
        qDebug() << record.field("UniqueID").value().toString();
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }
    currentlyplaying = row;

    //tablewidget->selectRow(row);
}


void LibraryController::playNextFile()
{
    currentlyplaying += 1;
    if (currentlyplaying >= currentdata->length())
    {
        currentlyplaying = 0;
    }
    QSqlRecord record = currentdata->at(currentlyplaying);
    //TODO: Add checking at the end
    QString filepath = record.field("FilePath").value().toString();
    qDebug() << "Currently playing: " << filepath;
    if(record.field("UniqueID").value() != "local")
    {
        qDebug() << "NOT LOCAL";
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }
    tablewidget->selectRow(currentlyplaying);
}

void LibraryController::playPrevFile()
{
    currentlyplaying -= 1;//Decrement by 1
    if (currentlyplaying < 0)
    {
        currentlyplaying = currentdata->length()-1;
    }
    QSqlRecord record = currentdata->at(currentlyplaying);
    QString filepath = record.field("FilePath").value().toString();
    qDebug() << "Currently playing: " << filepath;
    if(record.field("UniqueID").value() != "local")
    {
        qDebug() << "NOT LOCAL";
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }
    tablewidget->selectRow(currentlyplaying);

}

void LibraryController::displaythis(QList<QSqlRecord>* passedin)
{
  qDebug() << "Reached here";
  fillData(passedin);
}

LibraryController::~LibraryController()
{
    if(currentdata!=NULL)
    {
        delete currentdata;
    }

    if(sortcols!=NULL)
    {
        delete sortcols;
    }

    if(orders!=NULL)
    {
        delete orders;
    }

    delete paneldelegate;
}
