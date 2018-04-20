// Monitors concept
// Bartlomiej Kulik
// 20 April 2018

#include "monitor.h"

class Cars : public Monitor
{
private:

    const int PRIORITY, CARS, LAPS, SERVICES;
    int inService;
    bool isFreeServiceTrack;
    Condition entry, escape;

public:

    void entryToService() // monitor function
    {
        enter();

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
        // now i have service track
        sleep();
        ++inService;
        leaveServiceTrack();

        leave();
    } // entryToService()

    void escapeTheService() // monitor function
    {
        enter()

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
        // now i have service track
        sleep();
        --inService;
        leaveServiceTrack();

        leave();
    } // escapeTheService()

private:

    void leaveServiceTrack() // no monitor function!
    {
        if (inService < PRIORITY)
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
        else
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
};
