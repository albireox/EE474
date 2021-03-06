// leds.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "bool.h"
#include "leds.h"


// Base path to the BeagleBone LED directory. The first substitution is an
// integer from 0 to 3 indicating the LED to use. The second substitution is
// the file to which to write.
const char basePath[100] = "/sys/class/leds/beaglebone:green:usr%d/%s";


void writeToFile(char * path, char * text) {
    // Writes some text to a file.

    FILE *ff = fopen(path, "w");

    if (ff == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(ff, "%s", text);

    fclose(ff);

}

void setLED(unsigned short ledID, Bool on, unsigned int delay_on,
            unsigned int delay_off) {
    // Sets a LED with ledID to an on or off state. If delay_on or delay_off
    // are non-zero, sets the LED to blick on that interval.
    // See http://robotic-controls.com/learn/beaglebone/beaglebone-black-built-leds

    char path[300];
    char delay[20];

    if (on == FALSE) {  // Turns LED off.

        sprintf(path, basePath, ledID, "trigger");
        writeToFile(path, "none");

        sprintf(path, basePath, ledID, "brightness");
        writeToFile(path, "0");

    } else {

        if (delay_on == 0 & delay_off == 0) {  // Turns LED solid on.

            sprintf(path, basePath, ledID, "trigger");
            writeToFile(path, "none");

            sprintf(path, basePath, ledID, "brightness");
            writeToFile(path, "255");

        } else {  // Turns LED flashing.

            sprintf(path, basePath, ledID, "trigger");
            writeToFile(path, "timer");

            sprintf(path, basePath, ledID, "brightness");
            writeToFile(path, "255");

            sprintf(path, basePath, ledID, "delay_on");
            sprintf(delay, "%d", delay_on);
            writeToFile(path, delay);

            sprintf(path, basePath, ledID, "delay_off");
            sprintf(delay, "%d", delay_off);
            writeToFile(path, delay);

        }

    }

}


// Some convenience functions to control LEDs.

void turnOnLED(unsigned short ledID) {
    // Turns a LED on
    setLED(ledID, TRUE, 0, 0);
}


void turnOffLED(unsigned short ledID) {
    // Turns a LED off
    setLED(ledID, FALSE, 0, 0);
}


void flashLED(unsigned short ledID, unsigned int delay_on,
              unsigned int delay_off) {
    // Flashes a LED with interval on delay_on and off delay_off.
    setLED(ledID, TRUE, delay_on, delay_off);
}


void turnAllOff(){
    // Turns off all LEDs

    int ii;

    for (ii=0; ii<=3; ii++) {
        turnOffLED(ii);
    }

}


int leds_main() {
    // Tests for LEDs.

    unsigned short ii;

    for (ii=0; ii < 4; ii++){
        setLED(ii, FALSE, 0, 0);
    }

    usleep(3000000);

    for (ii=0; ii < 4; ii++){
        setLED(ii, TRUE, ii * 1000, ii * 1000);
        usleep(1000000);
    }

    return 0;

}
