#include "sleep.h"
#include <iostream>
#include <QtGlobal>

Sleep::Sleep()
{}

#ifdef Q_OS_WIN
#include "unistd.h" // for usleep
#endif

void Sleep::qSleep(unsigned int ms)
{

#ifdef Q_OS_WIN
    usleep(ms * 1000);

#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, nullptr);
#endif
}
