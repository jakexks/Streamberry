#ifndef SBEXCEPTION_H
#define SBEXCEPTION_H

//Streamberry Exception Class

#include <QString>

//enumerated error types
enum extype {GENERIC, DB};

//Streamberry exceptions
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
