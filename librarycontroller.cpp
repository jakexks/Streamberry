#include "librarycontroller.h"
#include "utilities.h"
#include "albumpanel.h"
#include <QtGui>
#include <QString>
#include <QGridLayout>

LibraryController::LibraryController(Utilities& utilities)
    : util(utilities)
{
    widget = makeWidget();
}

QWidget* LibraryController::getWidget()
{
    return widget;
}

QWidget* LibraryController::makeWidget()
{
    QWidget *tempw = new QWidget();
    tempw->setObjectName("rightSideWidget");
    tempw->setStyleSheet("QWidget#rightSideWidget { background-color: #FFFFFF }");

    QGridLayout *temp = new QGridLayout(tempw);
    temp->setMargin(0);

    // librarygenerator lg; This was horribly broken and reverted...
    //albuminfo ai;
    QTableWidget *tablewidget = new QTableWidget(12, 3);
    tablewidget->setObjectName("libraryTableWidget");
    tablewidget->setGridStyle(Qt::NoPen);
    tablewidget->verticalHeader()->setVisible(false);
    //tablewidget->setRowCount(ai.getTracks().length());
    tablewidget->setColumnCount(3);
    //tablewidget->setSpan(0,0,ai.getTracks().length(),1);
    //tablewidget->setItem(0, 1, new QTableWidgetItem(ai.getTitle()));
    //tablewidget->setItem(1, 1, new QTableWidgetItem(ai.getArtist()));
    tablewidget->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    tablewidget->setColumnWidth(0, 130);
    tablewidget->setColumnWidth(2, 250);

    QTableWidgetItem *headertemp = new QTableWidgetItem("Title", 0);
    tablewidget->setHorizontalHeaderItem(2, headertemp);
    headertemp = new QTableWidgetItem("", 0);
    tablewidget->setHorizontalHeaderItem(1, headertemp);
    headertemp = new QTableWidgetItem("", 0);
    tablewidget->setHorizontalHeaderItem(0, headertemp);
    AlbumPanel *albumpanel = new AlbumPanel(util.getExecutePath());
    tablewidget->setCellWidget(0, 0, albumpanel);
    tablewidget->verticalHeader()->setVisible(FALSE);;
    temp->addWidget(tablewidget);

    return tempw;
}
