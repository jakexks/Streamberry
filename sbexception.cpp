#include "sbexception.h"
#include <exception>

SBException::SBException()
{
    type = GENERIC;
    exmsg = "";
}

SBException::SBException(extype t)
{
    type = t;
    exmsg = "";
}

SBException::SBException(extype t, QString msg)
{
    type = t;
    exmsg = msg;
}

QString SBException::getException()
{
    QString s;

    switch(type) {
    case GENERIC:
        s += "Generic";
    case DB:
        s += "Database";
    }

    s += " exception.";

    if(s!="") {
        s += " '";
        s += exmsg;
        s += "'.";
    }

    return s;
}
