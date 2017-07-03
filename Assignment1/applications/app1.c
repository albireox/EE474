// app1.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>


// Function prototypes
void delay(unsigned int a_delay);


// Global variables
int flash_delay = 1;  // Defines how many seconds to delay.


int main() {
    // Writes A B C D and flashes them on a one-second interval.

    // Initial delay
    delay(flash_delay);

    while(1) {

        // Prints and flushes A B C D with a carriage returns.
        printf("%cA B C D", 0x0d);
        fflush(stdout);

        // Waits flash_delay seconds
        delay(flash_delay);

        // Clears the line.
        printf("%c       ", 0x0d);
        fflush(stdout);

        // Waits flash_delay seconds
        delay(flash_delay);

    }

}
