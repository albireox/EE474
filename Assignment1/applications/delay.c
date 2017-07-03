// delay.c
//
// Conor Sayres and José Sánchez-Gallego


#include <time.h>
#include <stdio.h>

void delay (unsigned int a_delay) {
    // Waits a_delay seconds before returning.

    unsigned int return_time = time(0) + a_delay;  // Return time
    while (time(0) < return_time);                 // Loops until return time

}
