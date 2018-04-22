// Monitors cars solution
// Bartlomiej Kulik
// 22 April 2018

#ifndef __monitorsCarsSolution
#define __monitorsCarsSolution

#include "monitor.h"
#include <unistd.h>
#include <iostream>

class CarsSynchrinize : public Monitor
{
private:

    const unsigned int PRIORITY, SERVICES;
    unsigned int inService;
    bool isFreeServiceTrack;
    Condition entry, escape;

public:

    CarsSynchrinize(const unsigned int& priority, const unsigned int& services)
    :
    PRIORITY(priority), SERVICES(services),
    inService(0), isFreeServiceTrack(true)
    {}

    void entryToService(const unsigned int& trackTime)
    // monitor function
    {
        enter(); // take the critical section

        if (isFreeServiceTrack && inService < SERVICES)
        { // everything is ok to entry
            if (inService < PRIORITY) // entry priority
            {
                isFreeServiceTrack = false;
            }
            else // escape priority
            {
                if (!escape.isEmpty())
                {
                    entry.wait();
                    isFreeServiceTrack = false;
                }
                else
                {
                    isFreeServiceTrack = false;
                }
            }
        }
        else
        {
            entry.wait();
            isFreeServiceTrack = false;
        }
        // now I have the service track
        ++inService;
        sleep(trackTime);

        leaveServiceTrack();

        leave(); // release the critical section

    } // entryToService()

    void escapeTheService(const unsigned int& trackTime)
    // monitor function
    {
        printf("before enter\n");
        enter(); // take the critical section
        printf("after enter\n");

            if (isFreeServiceTrack)
            { // everything is ok to escape
                if (inService >= PRIORITY) // escape priority
                {
                    isFreeServiceTrack = false;
                }
                else // entry priority
                {
                    if (!entry.isEmpty())
                    {
                        escape.wait();
                        isFreeServiceTrack = false;
                    }
                    else
                    {
                        isFreeServiceTrack = false;
                    }
                }
            }
            else
            {
                escape.wait();
                isFreeServiceTrack = false;
            }
        // now i have the service track
        sleep(trackTime);
        --inService;
        leaveServiceTrack();

        leave(); // release the critical section
    } // escapeTheService()

private:

    void leaveServiceTrack()
    {
        if (inService < PRIORITY) // entry priority
        {
            if (!entry.isEmpty())
            {
                entry.signal();
            }
            else if (!escape.isEmpty())
            {
                escape.signal();
            }
            else // entry.isEmpty() && escape.isEntry() is true
            {
                isFreeServiceTrack = true;
            }
        }
        else // escape priority
        {
            if (!escape.isEmpty())
            {
                escape.signal();
            }
            else if (!entry.isEmpty() && inService < SERVICES)
            {
                entry.signal();
            }
            else
            {
                isFreeServiceTrack = true;
            }
        }
    } // leaveServiceTrack()

};

void lap(const unsigned int& lapTime)
{
    sleep(lapTime);
}

void service(const unsigned int& serviceTime)
{
    sleep(serviceTime);
}

#endif
