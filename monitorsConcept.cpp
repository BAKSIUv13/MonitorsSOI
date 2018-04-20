// Monitors concept
// Bartlomiej Kulik
// 18 April 2018

class cars : public monitor
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
                isFreeServiceTrack = true;
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
        // now i have service track
        sleep();
        ++inService;
        leaveServiceTrack();
        }

    }

    void escapeTheService()
    {
        if (isFreeServiceTrack)
        { // everything is ok to entry
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
        // now i have service track
        sleep();
        --inService;
        leaveServiceTrack();
    }

private:

    void leaveServiceTrack() // no monitor function
    {
        if (entry.isEmpty() && escape.isEmpty()) // nobody want to drive
        {
            isFreeServiceTrack = true;
            // without any signal()
        }
        else
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
                else
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
    }
};
