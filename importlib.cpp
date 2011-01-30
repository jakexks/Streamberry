#include "importlib.h"

using namespace rapidxml;

importLib::importLib(const char* fileName)
{
    QFileInfo info1(fileName);
    if(!info1.exists()) throw SBException(GENERIC, "File does not exist");
    file<char> file(fileName);
    doc.parse<0>(file.data());
}

QList<QString> importLib::libFileList(){
    QList<QString> list;
    xml_node<> *node = doc.first_node();
    node = node->first_node();
    node = node->first_node("dict");
    node = node->first_node();

    while (node!=0){
        list.append(getNext(node));
        node = node->next_sibling();
        if (node!=0) node = node->next_sibling();
    }

    //qDebug() << list.takeFirst() << endl;
}

QString importLib::getNext(xml_node<> *node){
    node = node->next_sibling();
    node = node->first_node();
    while((node = node->next_sibling())!=0){
        if(((string)node->value()) == "File"){
            node = node->next_sibling();
            node = node->next_sibling();
            return (QString)node->value();
        } else {
            continue;
        }
    }
    return "";
}
