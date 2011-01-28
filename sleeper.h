#ifndef SLEEPER_H
#define SLEEPER_H
#include <QThread>
class sleeper : public QThread
{
public:
    sleeper();
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

#endif // SLEEPER_H
