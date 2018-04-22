// Monitors cars solution - main fucntion
// Bartlomiej Kulik
// 22 April 2018

#include <iostream>
#include <stdio.h>
#include "monitorsCarsSolution.h"

#define _PRIORITY 2
#define _CARS 5
#define _SERVICES 3
#define _LAPS 1

#define _L_TIME 4
#define _S_TIME 4
#define _T_TIME 2

CarsSynchrinize formulaOne(_PRIORITY, _SERVICES);

void* car(void* id)
{
    for (int i = 0; i < _LAPS; ++i)
    {
        lap(_L_TIME);

        printf("before entry to service: %d\n", id);
        formulaOne.entryToService(_T_TIME);
        printf("after  entry to service: %d\n", id);

        printf("before service works: %d\n", id);
        service(_S_TIME);
        printf("after  service works: %d\n", id);

        printf("before escapeTheService: %d\n", id);
        formulaOne.escapeTheService(_T_TIME);
        printf("after  escapeTheService: %d\n", id);
    }

    return nullptr;
}

int main(void)
{
    pthread_t tid[_CARS];

    // go!
    for (int i = 0; i < _CARS; ++i)
    {
        pthread_create(&(tid[i]), NULL, car, (void*)i );
    }
    // stop!
    for (int i = 0; i < _CARS; ++i)
    {
        pthread_join(tid[i],(void **)NULL);
    }
    return 0;
}
