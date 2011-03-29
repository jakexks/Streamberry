#include "sharingtab.h"

#include <QDebug>

SharingTab::SharingTab(QStringList sfiles, QWidget *parent) : selectedfiles(sfiles), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    model = new QFileSystemModel;
    model->setRootPath("");
    tree = new QTreeView(this);
    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);

    int sflength = selectedfiles.length();
    for(int i = 0; i < sflength; i++)
    {
        tree->selectionModel()->select(model->index(selectedfiles.at(i)),QItemSelectionModel::SelectCurrent);
    }
    tree->setColumnWidth(0,300);
    tree->setFixedSize(500,550);
    layout->addWidget(tree);
    setLayout(layout);
}

SharingTab::~SharingTab()
{
    delete model;
}

QString SharingTab::getSelectedFilepaths()
{
    QModelIndexList sindexes = tree->selectionModel()->selectedRows();
    int sindlength = sindexes.length();
    QStringList strs;
    for(int i = 0; i < sindlength; i++)
    {
        strs.append(model->filePath(sindexes.at(i)));
    }
    return strs.join(";");
}
