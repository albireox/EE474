// leds.c
//
// Conor Sayres and José Sánchez-Gallego

#include "bool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


const char basePath[100] = "/sys/class/leds/beaglebone:green:usr%d/%s";


void writeToFile(char * path, char * text) {

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

    char path[300];
    char delay[20];

    if (on == FALSE) {

        sprintf(path, basePath, ledID, "trigger");
        writeToFile(path, "none");

        sprintf(path, basePath, ledID, "brightness");
        writeToFile(path, "0");

    } else {

        if (delay_on == 0 & delay_off == 0) {

            sprintf(path, basePath, ledID, "trigger");
            writeToFile(path, "none");

            sprintf(path, basePath, ledID, "brightness");
            writeToFile(path, "255");

        } else {

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


int main() {

    unsigned short ii;

    for (ii=0; ii < 4; ii++){
        setLED(ii, FALSE, 0, 0);
    }

    usleep(3000000);

    for (ii=0; ii < 4; ii++){
        setLED(ii, TRUE, ii * 1000, ii * 1000);
        usleep(1000000);
    }

}
