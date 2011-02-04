#ifndef SBEXCEPTION_H
#define SBEXCEPTION_H

//Streamberry Exception Class
//NOTE: COULD NOT WORK OUT HOW TO DO EXCEPTIONS. THIS PROBABLY NEEDS TO BE REWRITTEN PROPERLY.

#include <exception>
#include <QString>

//enumerated error types
enum extype {GENERIC, DB};

class SBException
{
public:
    SBException();
    SBException(extype t);
    SBException(extype t, QString msg);
    //returns string explaining exception that occurred
    QString getException();

private:
    extype type;
    QString exmsg;
};

#endif // SBEXCEPTION_H
