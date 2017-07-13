// satellite.h
//
// Conor Sayres and José Sánchez-Gallego
//
// This header file declares the tasks to be scheduled and function prototypes
// for each task, as well as global variables

#ifndef SATELLITE_H
#define SATELLITE_H

#include "bool.h"
#include <time.h>


// Global variables
extern unsigned int THRUST_CMD;           // Thruster command, allowed values?
extern unsigned short BATTERY_LVL;        // Battery level, percent?
extern unsigned short FUEL_LVL;           // Fuel level, percent?
extern unsigned short PWR_CONSUMPTION;    // Power consumption, what units
extern unsigned short PWR_GENERATION;     // Power generation, what units?

extern Bool SOLAR_PANEL_STATE;            // Solar panel state, FALSE means not deployed?
extern Bool FUEL_LOW;                     // Fuel low warning
extern Bool BATTERY_LOW;                  // Battery low warning


// TCB structure implementation
struct TCB {
    void (*myTask)(void*);
    void * taskDataPtr;
};


// Task data structure prototypes
struct WarningStruct {
    double lastTimeRun;
    float interval;
    Bool * fuelLow;
    Bool * batteryLow;
    unsigned short * fuelLevel;
    unsigned short * batteryLevel;
};


// Task function prototypes
void warningAlarmTask(void * data);


// General function prototypes

// From time.c
double now();
Bool doRun(float interval, double lastTimeRun);


#endif
