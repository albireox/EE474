// time.c
//
// Conor Sayres and José Sánchez-Gallego

#include <time.h>

#include "bool.h"


double now() {
    // Returns the current execution time. This time is not affected by usleep.
    // See https://linux.die.net/man/3/clock_gettime

    struct timespec spec;

    clock_gettime(CLOCK_MONOTONIC, &spec);

    return (double) (spec.tv_sec + spec.tv_nsec / 1e9);
}


Bool doRun(float interval, double lastTimeRun) {
    // Checks whether enough time has passed and a task should be run.

    if (now() - lastTimeRun > interval)
        return TRUE;
    else
        return FALSE;

}
