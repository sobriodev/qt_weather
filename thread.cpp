#include "thread.h"

/*!
 * \class Thread
 * \inmodule core
 * \brief The Thread class provides a way to do some tasks parallel
 */

/*!
 * \brief Thread constructor with optional \a parent object
 */
Thread::Thread(QObject *parent) : QThread(parent) {}

/*!
 * \internal
 * \brief Thread behaviour
 */
void Thread::run()
{
    while (isRunning()) {
        emit tick();
        msleep(1);
    }
}

/*!
 * \fn volid Thread::tick()
 * \brief The tick signal is emmited with 1ms frequency
 */
