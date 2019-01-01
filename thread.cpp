#include "thread.h"


/*!
 * \brief Thread constructor
 * \param parent Parent object
 */
Thread::Thread(QObject *parent) : QThread(parent) {}

/*!
 * \internal
 * \brief Thread behaviour
 */
void Thread::run()
{
    while(isRunning()) {
        emit tick();
        msleep(1);
    }
}

/*!
 * \fn Thread::tick()
 * \brief The tick signal is emmited with 1ms frequency
 */
