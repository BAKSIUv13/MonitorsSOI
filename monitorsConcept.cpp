// Monitors concept
// Bartlomiej Kulik
// 20 April 2018

class Cars : public Monitor
{
private:

    const int PRIORITY, CARS, LAPS, SERVICES;
    int inService;
    condition entry, escape;
    bool isFreeServiceTrack;

public:

    void entryToService()
    {
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

    } // entryToService()

    void escapeTheService()
    {
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
