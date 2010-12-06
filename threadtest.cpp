#include "threadtest.h"
#include <iostream>

int tid;

threadtest::threadtest(int id)
{
    tid = id;
}

void threadtest::run()
{
    while (true)
    {
    std::cout << "data from " << tid << std::endl;
    sleep(1);
    }
}

int threadtest::setid(int id)
{
    tid = id;
    return tid;
}
