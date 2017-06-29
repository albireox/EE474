// app3.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>

#define TRUE 1
#define FALSE 0

// Function prototypes
void delay(unsigned int a_delay);


// Global variables
int write_delay = 1;  // Defines how many seconds to delay.


int main() {
    // Writes A B C D. Flashes A C every seccond and B D every two seconds.

    // Switches to decide whether to print AC and BD
    int write_AC = TRUE;
    int write_BD = TRUE;

    // A counter to know in which iteration we are.
    int iteration = 0;
    unsigned int ii;


    // Leters to print.
    char letters[] = "ABCD";
    char letter;

    // Initial delay
    delay(write_delay);

    while(1) {

        for (ii=0; ii<sizeof(letters); ii++) {

            // Selects the letter to (maybe) print
            letter = letters[ii];

            // If the letter is A or C and write_AC is TRUE, prints the letter.
            if (letter == 'A' || letter == 'C'){
                printf("%c ", (write_AC ) ? letter : ' ');
            } else {  // Same for B and D.
                printf("%c ", (write_BD ) ? letter : ' ');
            }

        }

        // Prints a carriage return
        printf("%c", 0x0d);
        fflush(stdout);

        // Now we decide what letters to print in the next loop iteration.
        // For AC we change the show/no show status in each iteration.
        write_AC = !write_AC;

        // For B and D we change the status every two iterations.
        if (iteration % 2 != 0) {
            write_BD = !write_BD;
        }

        // Increases the iteration counter.
        iteration ++;

        // Waits for a second.
        delay(write_delay);

    }

}
