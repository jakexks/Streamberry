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
#include "playbackcontroller.h"
#include <QGradient>
#include <QContextMenuEvent>

#define DEFAULT_WIDTH 170
#define DEFAULT_ARTPANEL_WIDTH 140

LibraryController::LibraryController(Utilities& utilities, Database& datab, Player& p, SBSearchBar* searchbar)
    : util(utilities), db(datab), player(p)
{


    curheaders = NULL;
    currentlyplaying = -1;
    musicvideofilter = 2;
    playingdata = NULL;
    allwidgets = NULL;

    ViewQueueItem item;
    item.playlist = "";
    item.smarttext = "";
    item.searchtext = "";
    item.videoview = 0;
    viewqueue.append(item);
    viewqueueindex = 0;

    QStringList headers;
    QString headerstr;

    if((headerstr = db.getSetting("TableHeaders"))==NULL)
    {
        headers.append("Title");
        headers.append("Length");
        headers.append("Artist");
        headers.append("Album");
        headers.append("Genre");
        db.storeSetting("TableHeaders", "Title|Length|Artist|Album|Genre");
    }
    else
    {
        headers = headerstr.split("|", QString::SkipEmptyParts);
    }
    setHeaders(headers, 3);
    widget = new QWidget();
    widget->setObjectName("LibraryBackgroundWidget");
    widget->setStyleSheet("#LibraryBackgroundWidget { background:black; }");

    container = new QGridLayout(widget);
    container->setMargin(0);
    curview = NULL;
    currentdata = NULL;
    paneldelegate = new AlbumArtDelegate(util);
    makeWidget();

    QWidget* playerwind = player.initVid();


    allwidgets = new QStackedWidget();
    allwidgets->addWidget(curview);
    allwidgets->addWidget(playerwind);
    container->addWidget(allwidgets, 0, 0);

    resetQueue();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    shuffle=0;
    repeat=0;
    numberiterator=-1;

    trackmenu = new TrackContext(&db);

    QObject::connect(tablewidget->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(sectionResized(int,int,int)));
    QObject::connect(&player, SIGNAL(getFirstSong(int )), this, SLOT(itemClicked(int)));
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
    tablewidget->setMouseTracking(true);
    temp->addWidget(tablewidget);
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    tablewidget->setItemPrototype(item);

    addHeaders();

    tablewidget->setItemDelegateForColumn(0, paneldelegate);


    tablewidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(tablewidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    QObject::connect(tablewidget, SIGNAL(itemSelectionChanged(void)), this, SLOT(deselectFirst(void)));
    QObject::connect(tablewidget->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortIndicatorChanged(int,Qt::SortOrder)));
    QObject::connect(tablewidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemClicked(int)));
    //QObject::connect(tablewidget, SIGNAL(cellPressed(int,int)), this, SLOT(itemSelected(int)));
    QObject::connect(tablewidget, SIGNAL(cellEntered(int,int)), this, SLOT(cellrolled(int, int)));


    //QObject::connect(tablewidget, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(DragStart(QTableWidgetItem*)));

    tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
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
    tablewidget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    tablewidget->horizontalHeader()->setMovable(false);

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
            switch(i)
            {
            case 0:
            case 2:
            case 3:
                tablewidget->setColumnWidth(i+2, DEFAULT_WIDTH);
                break;
            case 1:
                tablewidget->setColumnWidth(i+2, 60);
                break;
            case 4:
                tablewidget->setColumnWidth(i+2, 100);
                break;
            }
        }

        tablewidget->setHorizontalHeaderItem(i+2, header);
    }
    tablewidget->horizontalHeader()->setMovable(true);
}

void LibraryController::fillData(QList<QSqlRecord> *values)
{
    if(currentdata!=NULL)
    {
        if(currentdata != playingdata)
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

#ifdef Q_WS_WIN
    font.setPointSize(9);
#else
    font.setPointSize(11);
#endif

    for(int i = 0; i<length; i++)
    {
        QSqlRecord currecord = values->at(i);
        QString curalbum = currecord.field(*curheaders[sortcolumn]).value().toString();
        //if the album has changed, start a new left column
        if(curalbum!=lastalbum)
        {
            if(albumstart>-1)
            {
                if( (i-albumstart)!=1 )
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
            //temp string to deal with track length (no leading 0)
            QString curitemtxt = currecord.field(*curheaders[j]).value().toString();
            if(curitemtxt.indexOf("0")==0) curitemtxt.remove(0,1);
            item = new QTableWidgetItem(curitemtxt);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            item->setFont(font);

            if(i%2==1)
                item->setBackgroundColor(background);
            else
                item->setBackgroundColor(background2);

            tablewidget->setItem(i, j+2, item);
        }
    }
    //delete values;

    if(length>0)
    {
        if( (length-albumstart)!=1 )
            tablewidget->setSpan(albumstart, 0, (length-albumstart), 1);
    }

    tablewidget->resizeColumnToContents(1);

    int rowtoselect = rowToHighlight();
    if(rowtoselect != -1)
    {
        tablewidget->selectRow(rowtoselect);
    }

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

    //    sortIndicatorChanged(sortcol+2, sortorder);
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
    QList<QString> sortcols;
    QList<QString> orders;
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
            sortcols.append("Artist");
            sortcols.append("Album");
            sortcols.append("Track");
            orders.append(orderstr);
            orders.append("ASC");
            orders.append("ASC");
        }
        else if(*curheaders[index-2] == "Album")
        {
            sortcols.append("Album");
            sortcols.append("Track");
            orders.append(orderstr);
            orders.append("ASC");
        }
        else if(*curheaders[index-2] == "Genre")
        {
            sortcols.append("Genre");
            sortcols.append("Artist");
            sortcols.append("Album");
            sortcols.append("Track");
            orders.append(orderstr);
            orders.append("ASC");
            orders.append("ASC");
            orders.append("ASC");
        }
        else if(*curheaders[index-2] == "Length")
        {
            sortcols.append("Length");
            sortcols.append("Track");
            orders.append(orderstr);
            orders.append("ASC");
        }
        else
        {
            sortcols.append(*curheaders[index-2]);
            orders.append(orderstr);
        }

        sortcolumn = index - 2;
        sortorder = order;

        viewqueue[viewqueueindex].sortcols = sortcols;
        viewqueue[viewqueueindex].orders = orders;

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

    if(searchtext!=viewqueue[viewqueueindex].searchtext)
    {
        ViewQueueItem item;
        item.playlist = viewqueue[viewqueueindex].playlist;
        item.smarttext = viewqueue[viewqueueindex].smarttext;
        item.searchtext = text;
        item.sortcols = viewqueue[viewqueueindex].sortcols;
        item.orders = viewqueue[viewqueueindex].orders;
        item.videoview = 0;

        while(viewqueueindex<viewqueue.size()-1)
        {
            viewqueue.removeLast();
        }

        viewqueue.append(item);
        viewqueueindex++;
    }

    updateLibrary();
}

void LibraryController::updateLibrary()
{
    if(tablewidget!=NULL)
    {
        if(viewqueue[viewqueueindex].videoview!=1)
        {
            QList<QSqlRecord> *result = db.searchDb(0, viewqueue[viewqueueindex].playlist, viewqueue[viewqueueindex].smarttext+" "+viewqueue[viewqueueindex].searchtext, viewqueue[viewqueueindex].sortcols, viewqueue[viewqueueindex].orders, musicvideofilter);
            fillData(result);
            emit setSelectedPlaylist(viewqueue[viewqueueindex].playlisttitle);
        }
        else
        {
            emit selectVideo();
        }

        if(allwidgets!=NULL && allwidgets->count()!=0)
        {
            allwidgets->setCurrentIndex(viewqueue[viewqueueindex].videoview);
        }
    }

}

void LibraryController::musicVideoFilter(int value)
{
    musicvideofilter = value;
    updateLibrary();
}

void LibraryController::itemClicked(int row)
{
    if(playingdata != NULL && playingdata != currentdata)
    {
        delete playingdata;
    }
    playingdata = currentdata;

    QSqlRecord record = playingdata->at(row);
    QString filepath = record.field("FilePath").value().toString();
    bool isvideo = record.field("MusicOrVideo").value().toInt();
    if(isvideo)
    {
        pushVideoView();
    }
    emit songInfoData(record.field("Album").value().toString(), record.field("Artist").value().toString(), record.field("Title").value().toString(), record.field("Track").value().toString());
    qDebug() << "Currently playing: " << filepath;
    if(record.field("UniqueID").value() != "Local")
    {
        qDebug() << "NOT LOCAL";
        qDebug() << record.field("UniqueID").value().toString();
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }
    qDebug() << "currentlyplaying " << currentlyplaying;
    currentlyplaying = row;
    makeShuffleList(currentlyplaying);
}

int LibraryController::rowToHighlight()
{
    if(currentlyplaying != -1)
    {
        const QSqlRecord playing = playingdata->at(currentlyplaying);
        int row = currentdata->indexOf(playing);
        return row;
    }
    return -1;
}


void LibraryController::playplaylist(QString playlistname)
{
    QList<QString> fields;
    QList<QString> order;
    fields.append("Album");
    order.append("DESC");
    QList<QSqlRecord>* data = db.searchDb(0, playlistname, "", fields,order, 0);
    if(playingdata!=NULL && playingdata != currentdata)
        delete playingdata;
    playingdata = data;
    if( !playingdata->isEmpty() )
    {
        QSqlRecord record = playingdata->at(0);
        tablewidget->selectRow(0);
        emit songInfoData(record.field("Album").value().toString(), record.field("Artist").value().toString(), record.field("Title").value().toString(), record.field("Track").value().toString());
        qDebug() << "Currently playing: " << record.field("FilePath").value().toString();
        if(record.field("UniqueID").value().toString() != "Local")
        {
            QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
            player.playFile(record.field("FilePath").value().toString(), record.field("UniqueID").value().toString(), ipaddress);
        } else {
            player.playFile(record.field("FilePath").value().toString());
        }
        makeShuffleList(-1);
    }
}

void LibraryController::playsmartplaylist(QString filter)
{
    QList<QString> fields;
    QList<QString> order;
    fields.append("Album");
    order.append("DESC");
    QList<QSqlRecord>* data = db.searchDb(0, "", filter, fields,order, 0);
    if(playingdata!=NULL && playingdata != currentdata)
        delete playingdata;
    playingdata = data;
    if( !playingdata->isEmpty() )
    {
        QSqlRecord record = playingdata->at(0);
        tablewidget->selectRow(0);
        emit songInfoData(record.field("Album").value().toString(), record.field("Artist").value().toString(), record.field("Title").value().toString(), record.field("Track").value().toString());
        qDebug() << "Currently playing: " << record.field("FilePath").value().toString();
        if(record.field("UniqueID").value().toString() != "Local")
        {
            QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
            player.playFile(record.field("FilePath").value().toString(), record.field("UniqueID").value().toString(), ipaddress);
        } else {
            player.playFile(record.field("FilePath").value().toString());
        }
        makeShuffleList(-1);
    }
}

void LibraryController::shuffleSlot()
{
    if(shuffle==0)
    {
        shuffle=1;
        makeShuffleList(-1);
    }
    else
        shuffle=0;
}

void LibraryController::makeShuffleList(int firstsong)

{
    maxsize=currentdata->length();
    shufflelist=new int[maxsize];
    int initial=1;
    if(firstsong!=-1)
        shufflelist[0]=firstsong;
    else
        initial=0;

    //initialise the elements of the array
    for(int i=initial; i<maxsize; i++)
        if(i==firstsong)
            shufflelist[i]=0;
    else
        shufflelist[i]=i;

    //shuffle the array
    int j=0;
    for(int i=maxsize-1; i>=0; i--)
    {
        j=randInt(0,i);
        int tmp=shufflelist[i];
        shufflelist[i]=shufflelist[j];
        shufflelist[j]=tmp;
    }

    //make the selected song be the first one in the list
    if(firstsong!=-1)
    {
        for(int i=0; i<maxsize; i++)
        {
            if(shufflelist[i]==firstsong)
            {
                int tmp=shufflelist[i];
                shufflelist[i]=shufflelist[0];
                shufflelist[0]=tmp;
            }
        }
    }
    //    for(int i=0; i<maxsize; i++)
    //    {
    //        qDebug()<<shufflelist[i];
    //    }
    numberiterator=0;
}

void LibraryController::repeatSlot(bool one, bool all)
{
    //repeat none
    if(one==false && all==false)
        repeat=0;

    //repeat all
    if(one==true && all==true)
        repeat=2;

    //repeat one
    if(one==true && all==false)
        repeat=1;

}

void LibraryController::playNextFile()
{
    if(currentlyplaying==-1) return;
    if(repeat!=1)
    {
        if(shuffle==1)
        {
            numberiterator+=1;
            if(numberiterator >=  maxsize)
                numberiterator=0;

            currentlyplaying=shufflelist[numberiterator];
        }

        if(shuffle==0)
            currentlyplaying += 1;

        if(currentlyplaying >=  playingdata->length()&&repeat==2)
            currentlyplaying = 0;
        if(currentlyplaying >= playingdata->length()&&repeat!=2)
        {
            currentlyplaying=-1;
            player.stopPlayer();
            emit songInfoData("","","","");
            return;
        }
    }

    QSqlRecord record = playingdata->at(currentlyplaying);
    QString filepath = record.field("FilePath").value().toString();
    bool isvideo = record.field("MusicOrVideo").value().toInt();
    if(isvideo)
    {
        pushVideoView();
    }
    qDebug() << "Currently playing: " << filepath;

    emit songInfoData(record.field("Album").value().toString(), record.field("Artist").value().toString(), record.field("Title").value().toString(), record.field("Track").value().toString());

    if(record.field("UniqueID").value() != "Local")
    {
        qDebug() << "NOT LOCAL";
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }

    if(currentdata == playingdata)
    {
        tablewidget->selectRow(currentlyplaying);
    }
}

void LibraryController::playPrevFile()
{

    if(currentlyplaying==-1) return;
    if(repeat!=1)
    {
        if(shuffle==1)
        {
            numberiterator--;
            if(numberiterator < 0)
                numberiterator=maxsize-1;

            currentlyplaying=shufflelist[numberiterator];
        }

        if(shuffle==0)
        {
            currentlyplaying -= 1;//Decrement by 1
        }
        if (currentlyplaying < 0&&repeat==2)
        {
            currentlyplaying = currentdata->length()-1;
        }
        if (currentlyplaying < 0&&repeat!=2)
            currentlyplaying++;
    }
    QSqlRecord record = playingdata->at(currentlyplaying);

    QString filepath = record.field("FilePath").value().toString();
    bool isvideo = record.field("MusicOrVideo").value().toInt();
    if(isvideo)
    {
        pushVideoView();
    }
    qDebug() << "Currently playing: " << filepath;
    emit songInfoData(record.field("Album").value().toString(), record.field("Artist").value().toString(), record.field("Title").value().toString(), record.field("Track").value().toString());

    if(record.field("UniqueID").value() != "Local")
    {
        qDebug() << "NOT LOCAL";
        QString ipaddress = db.getIPfromUID(record.field("UniqueID").value().toString());
        player.playFile(filepath, record.field("UniqueID").value().toString(), ipaddress);
    } else {
        player.playFile(filepath);
    }
    if(currentdata == playingdata)
    {
        tablewidget->selectRow(currentlyplaying);
    }
}



LibraryController::~LibraryController()
{
    if(currentdata!=NULL)
    {
        delete currentdata;
    }
    if(playingdata!=NULL)
    {
        delete playingdata;
    }
    delete paneldelegate;
    delete shufflelist;
}

void LibraryController::ShowContextMenu(const QPoint&)
{
    QList<QString> idlist;
    QList<QString> uniqueIdList;
    int i=0;
    QModelIndexList selected = tablewidget->selectionModel()->selectedIndexes();
    for(i=0; i<selected.size(); i=i+6)
    {
        int row = selected.at(i).row();
        QSqlRecord record = currentdata->at(row);
        idlist.append(record.field("Filepath").value().toString());
        uniqueIdList.append(record.field("UniqueID").value().toString());
    }
    trackmenu->trackRightClicked(idlist, uniqueIdList, this);
    updateLibrary();
}

void LibraryController::goBack()
{
    if(viewqueueindex>0)
    {
        viewqueueindex--;
        emit setSearchBoxText(viewqueue[viewqueueindex].searchtext);
        emit setSelectedPlaylist(viewqueue[viewqueueindex].playlisttitle);
        updateLibrary();
    }
}

void LibraryController::goForward()
{
    if(viewqueueindex<viewqueue.size()-1)
    {
        viewqueueindex++;
        emit setSearchBoxText(viewqueue[viewqueueindex].searchtext);
        emit setSelectedPlaylist(viewqueue[viewqueueindex].playlisttitle);
        updateLibrary();
    }
}

void LibraryController::pushAllView()
{
    if(viewqueue[viewqueueindex].playlist!="" || viewqueue[viewqueueindex].smarttext != "" || viewqueue[viewqueueindex].videoview != 0)
    {

        while(viewqueueindex<viewqueue.size()-1)
            viewqueue.removeLast();

        ViewQueueItem item;
        item.playlist = "";
        item.smarttext = "";
        item.searchtext = "";
        item.videoview = 0;
        viewqueue.append(item);
        viewqueueindex++;
        emit setSearchBoxText("");
        tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
        updateLibrary();
    }
}

void LibraryController::pushNormalPlaylist(QString name)
{
    if(viewqueue[viewqueueindex].playlist!=name)
    {
        while(viewqueueindex<viewqueue.size()-1)
            viewqueue.removeLast();

        ViewQueueItem item;
        item.playlist = name;
        item.playlisttitle = name;
        item.smarttext = "";
        item.searchtext = "";
        item.videoview = 0;
        viewqueue.append(item);
        viewqueueindex++;
        emit setSearchBoxText("");
        tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
    }
}

void LibraryController::pushVideoView()
{
    while(viewqueueindex<viewqueue.size()-1)
        viewqueue.removeLast();

    ViewQueueItem item;
    item.playlist = "";
    item.smarttext = "";
    item.searchtext = "";
    item.videoview = 1;
    viewqueue.append(item);
    viewqueueindex++;
    emit setSearchBoxText("");
    //  tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
    updateLibrary();
}

void LibraryController::pushSmartPlaylist(QString name, QString filtertext)
{
    if(viewqueue[viewqueueindex].playlist!=name)
    {
        while(viewqueueindex<viewqueue.size()-1)
            viewqueue.removeLast();

        ViewQueueItem item;
        item.playlist = "";
        item.playlisttitle = name;
        item.smarttext = filtertext;
        item.searchtext = "";
        item.videoview = 0;
        viewqueue.append(item);
        viewqueueindex++;
        emit setSearchBoxText("");
        tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
    }
}

int LibraryController::getCurrentViewType()
{
    if(viewqueue[viewqueueindex].smarttext == "" && viewqueue[viewqueueindex].playlist == "")
        return 0; //viewing all media
    if(viewqueue[viewqueueindex].smarttext == "" && viewqueue[viewqueueindex].playlist != "")
        return 1; //viewing normal playlist
    if(viewqueue[viewqueueindex].smarttext != "" && viewqueue[viewqueueindex].playlist == "")
        return 2; //viewing smart playlist
    return 0;
}

QString LibraryController::getCurrentPlaylistName()
{
    if(viewqueue[viewqueueindex].smarttext == "" && viewqueue[viewqueueindex].playlist != "")
        return viewqueue[viewqueueindex].playlist;
    return "";
}

int LibraryController::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

void LibraryController::resetQueue()
{
    viewqueue.empty();
    ViewQueueItem item;
    item.playlist = "";
    item.smarttext = "";
    item.searchtext = "";
    item.videoview = 0;
    viewqueue.append(item);
    viewqueueindex = 0;
    emit setSearchBoxText("");
    tablewidget->horizontalHeader()->setSortIndicator(sortcolumn+2, sortorder);
}

void LibraryController::cellrolled(int i, int j)
{
    if(j == 0)
        emit rollAlbum();
    else
        emit rolldefault();

}
