#ifndef THREADTEST_H
#define THREADTEST_H
#include <QThread>
#include <iostream>
class threadtest : public QThread
{
public:
    threadtest(int id);
    int setid(int id);

protected:
    void run();

private:
    int tid;
};

#endif // THREADTEST_H
