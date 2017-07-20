// gpio.c
//
// Conor Sayres and José Sánchez-Gallego
// Helper functions for setting gpio bits
// associated with auxiliary satellite hardware
#include <stdio.h>
#include <string.h>

// #include <stdio.h>
// #include <time.h>
// #include <unistd.h>
// #include "satellite.h"

#define THRUST_DOWN "/sys/class/gpio/gpio66/value"
#define THRUST_UP "/sys/class/gpio/gpio67/value"
#define THRUST_RIGHT "/sys/class/gpio/gpio68/value"
#define THRUST_LEFT "/sys/class/gpio/gpio69/value"

// gpio state flags for each thruster
// for on vs off, initialize all to off
static int gpioFlagDown = 0;
static int gpioFlagUp = 0;
static int gpioFlagLeft = 0;
static int gpioFlagRight = 0;



// create a file on disk containing 1 or 0
void genericSet(char * filePath, int value)
{
    char sysCmd[80];
    sprintf(sysCmd, "echo %i > ", value);
    strcat(sysCmd, filePath);
    int out = system("cat /sys/class/gpio/gpio69/value");
    printf("%i\n", out);
    system(sysCmd);
}

void setThrustDown(int value)
{
    static int lastValue = -1;
    if(value != lastValue)
    {
        lastValue = value;
        genericSet(THRUST_DOWN, value);
    }
}

void setThrustUp(int value)
{
    static int lastValue = -1;
    if(value != lastValue)
    {
        lastValue = value;
        genericSet(THRUST_UP, value);
    }
}

void setThrustLeft(int value)
{
    static int lastValue = -1;
    if(value != lastValue)
    {
        lastValue = value;
        genericSet(THRUST_LEFT, value);
    }
}

void setThrustRight(int value)
{
    static int lastValue = -1;
    if(value != lastValue)
    {
        lastValue = value;
        genericSet(THRUST_RIGHT, value);
    }
}