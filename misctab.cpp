#include "misctab.h"

MiscTab::MiscTab(QWidget *parent) : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout;

    QLineEdit *nickedit = new QLineEdit();
    layout->addRow("&Nickname:", nickedit);

    setLayout(layout);
}
