// app2.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>


// Function prototypes
void delay(unsigned int a_delay);


// Global variables
int write_delay = 1;  // Defines how many seconds to delay.


int main() {
    // Writes A B C D one by one and clears them on a one-second interval.

    char to_write[] = "\rABCD\r   ";  // The characters to write, including CR.
    unsigned int ii;

    // Initial delay
    delay(write_delay);

    while(1) {

        // Loops over the length of to_write
        for (ii=0; ii<sizeof(to_write); ii++) {

            // Prints the character. If the character is not a carriage return
            // also prints a space.
            printf("%c%c", to_write[ii], (to_write[ii] != '\r') ? ' ' : '\0');
            fflush(stdout);

            // We want to wait only for letters, not for the carriage return.
            if (to_write[ii] != '\r') {
                delay(write_delay);
            }

        }

    }

}
