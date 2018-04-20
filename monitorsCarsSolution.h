// Monitors cars solution
// Bartlomiej Kulik
// 20 April 2018

#ifndef __monitorsCarsSolution
#define __monitorsCarsSolution

#include "monitor.h"
#include <unistd.h>

class Cars : public Monitor
{
private:

    const unsigned int PRIORITY, CARS, LAPS, SERVICES;
    int inService;
    bool isFreeServiceTrack;
    Condition entry, escape;

public:

    Cars(const unsigned int& priority, const unsigned int& cars,
        const unsigned int& laps, const unsigned int& services) :
        PRIORITY(priority), CARS(cars), LAPS(laps), SERVICES(services), inService(0), isFreeServiceTrack(true)
    {}

    void bolid(const unsigned int& id, const unsigned int& lapTime,
              const unsigned int& serviceTime,
              const unsigned int& serviceTrackTime)
    {
        for (unsigned int i = 0; i < LAPS; ++i)
        {
            drive(id, lapTime);
            entryToService(id, serviceTrackTime);
            serviceOperations(id, serviceTime);
            escapeTheService(id, serviceTrackTime);
        }
    }

private:

    void entryToService(const unsigned int& id) // monitor function
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
        }
        // now I have the service track
        ++inService;
        drive(2);
        leaveServiceTrack();

        leave(); // release the critical section
    } // entryToService()

    void escapeTheService(const unsigned int& id) // monitor function
    {
        enter(); // take the critical section

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
            }
        // now i have the service track
        drive(2);
        --inService;
        leaveServiceTrack();

        leave(); // release the critical section
    } // escapeTheService()

    void leaveServiceTrack(const unsigned int& id)
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
            else if (!entry.isEmpty())
            {
                entry.signal();
            }
            else
            {
                isFreeServiceTrack = true;
            }
        }
    } // leaveServiceTrack()

    void drive(const unsigned int& id, const unsigned int& time)
    {
        sleep(time);
    }

    void serviceOperations(const unsigned int& id, const unsigned int& time)
    {
        sleep(time);
    }

};

#endif
