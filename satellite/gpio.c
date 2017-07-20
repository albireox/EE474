// gpio.c
//
// Conor Sayres and José Sánchez-Gallego
// Helper functions for setting gpio bits
// associated with auxiliary satellite hardware
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// #include <stdio.h>
// #include <time.h>
// #include <unistd.h>
// #include "satellite.h"

#define THRUST_DOWN "/sys/class/gpio/gpio66/value"
#define THRUST_UP "/sys/class/gpio/gpio67/value"
#define THRUST_RIGHT "/sys/class/gpio/gpio68/value"
#define THRUST_LEFT "/sys/class/gpio/gpio69/value"
#define BATTERY_LEVEL "/sys/devices/ocp.3/helper.15/AIN0"

// int out = system("cat /sys/class/gpio/gpio69/value");
// printf("%i\n", out);

// check whether or not a file exists on disk
// return 1 or 0
int fileExists(char * filePath)
{
    if( access (filePath, F_OK) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// read the contents of a file as an integer,
// and return it
int readInt(char * filePath)
{
    char sysCmd[80];
    sprintf(sysCmd, "cat ");
    strcat(sysCmd, filePath);  // cat the file, save output as int
    int outInt = system(sysCmd);
    return outInt
}


// create a file on disk containing 1 or 0
void genericSet(char * filePath, int value)
{
    char sysCmd[80];
    sprintf(sysCmd, "echo %i > ", value);
    strcat(sysCmd, filePath);
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

// return battery level 0-100
// read from the adc
// adc returns a value in millivolts
// max is 1800, convert it to a percentage
// return it as an unsigned short
unsigned short getBatteryLevel()
{
    float mv = (float)readInt(BATTERY_LEVEL);
    float percentage = mv / 1800.0 * 100.0;
    if(percentage > 100.0)
    {
        percentage = 100.0
    }
    return (unsigned short)percentage;
}






