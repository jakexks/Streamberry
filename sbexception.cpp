#include "sbexception.h"

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

    switch(type)
    {
        case GENERIC:
            s += "Generic";
            break;
        case DB:
            s += "Database";
            break;
    }

    s += " exception.";

    if(exmsg!="")
    {
        s += " '";
        s += exmsg;
        s += "'.";
    }

    return s;
}
