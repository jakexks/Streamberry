#ifndef IMPORTLIB_H
#define IMPORTLIB_H

#include <QString>
#include <QList>
#include <QFileInfo>
#include "rapidxml.hpp"
#include "sbexception.h"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

class importLib
{
public:
    importLib(const char* fileName);
    QList<QString> libFileList();
private:
    QString getNext(xml_node<> *node);
    xml_document<> doc;
};

#endif // IMPORTLIB_H
