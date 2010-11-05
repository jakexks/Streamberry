#ifndef SBEXCEPTION_H
#define SBEXCEPTION_H

//NOTE: COULD NOT WORK OUT HOW TO DO EXCEPTIONS. THIS PROBABLY NEEDS TO BE REWRITTEN PROPERLY.

#include <exception>
#include <QString>

enum extype {GENERIC, DB};

class SBException
{
public:
    SBException();
    SBException(extype t);
    SBException(extype t, QString msg);
    QString getException();

private:
    extype type;
    QString exmsg;
};

#endif // SBEXCEPTION_H
