// time.c
//
// Conor Sayres and José Sánchez-Gallego

#include <time.h>

#include "bool.h"


double now() {
    // Returns the current execution time.

    struct timespec spec;

    clock_gettime(CLOCK_MONOTONIC, &spec);
    // clock_t tt = clock();
    //
    // return (double) (tt / CLOCKS_PER_SEC);
    return (double) (spec.tv_sec + spec.tv_nsec / 1e9);
}


Bool doRun(float interval, double lastTimeRun) {

    if (now() - lastTimeRun > interval)
        return TRUE;
    else
        return FALSE;

}
