// time.c
//
// Conor Sayres and José Sánchez-Gallego

#include <time.h>

#include "bool.h"


double now() {
    // Returns the current execution time.

    clock_t tt = clock();

    return (double) (tt / CLOCKS_PER_SEC);

}


Bool doRun(float interval, double lastTimeRun) {

    if (now() - lastTimeRun > interval)
        return TRUE;
    else
        return FALSE;

}
