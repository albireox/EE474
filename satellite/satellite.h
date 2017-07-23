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
extern unsigned int THRUST_CMD;           // Thruster command,
extern unsigned short BATTERY_LVL;        // Battery level, percent?
extern unsigned short FUEL_LVL;           // Fuel level, percent?
extern unsigned short PWR_CONSUMPTION;    // Power consumption
extern unsigned short PWR_GENERATION;     // Power generation

extern Bool SOLAR_PANEL_STATE;            // Solar panel state, TRUE means not deployed
extern Bool SOLAR_PANEL_DEPLOY;
extern Bool SOLAR_PANEL_RETRACT;
extern Bool SOLAR_PANEL_SPEED_INC;
extern Bool SOLAR_PANEL_SPEED_DEC;
extern Bool FUEL_LOW;                     // Fuel low warning
extern Bool BATTERY_LOW;                  // Battery low warning

// vehicle comms globals
extern char VEHICLE_COMMAND[3];
extern char VEHICLE_RESPONSE[3];


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

struct SatelliteCommsStruct {
    double lastTimeRun;
    float interval;
    unsigned short* batteryLvlPtr;
    unsigned short* pwrConsumptionPtr;
    unsigned short* pwrGenerationPtr;
    unsigned short* fuelLvlPtr;
    Bool* solarPanelStatePtr;
    Bool* fuelLowPtr;
    Bool* batteryLowPtr;
    unsigned int* thrustCmdPtr;
};

struct ThrusterSubsystemStruct {
    double lastTimeRun;
    float interval;
    unsigned short* fuelLvlPtr;
    unsigned int* thrustCmdPtr;
};


struct ConsoleStruct {
    double lastTimeRun;
    float interval;
    Bool * fuelLow;
    Bool * batteryLow;
    unsigned short * fuelLevel;
    unsigned short * batteryLevel;
    Bool * solarPanelState;
    unsigned short * powerConsumption;
    unsigned short * powerGeneration;
};

struct PowerSubsystemStruct {
    double lastTimeRun;
    float interval;
    unsigned short* batteryLvlPtr;
    unsigned short* pwrConsumptionPtr;
    unsigned short* pwrGenerationPtr;
    Bool* solarPanelStatePtr;
    Bool* solarPanelDeployPtr;
    Bool* solarPanelRetractPtr;
};

struct SolarPanelStruct {
    double lastTimeRun;
    float interval;
    Bool* solarPanelStatePtr;
    Bool* solarPanelDeployPtr;
    Bool* solarPanelRetractPtr;
    Bool* solarPanelSpeedIncPtr;
    Bool* solarPanelSpeedDecPtr;
};

struct VehicleCommsStruct {
    double lastTimeRun;
    float interval;
    char* vehicleCommandPtr;
    char* vehicleResponsePtr;
};

struct ConsoleKeypadStruct {
    double lastTimeRun;
    float interval;
    Bool* solarPanelSpeedIncPtr;
    Bool* solarPanelSpeedDecPtr;
};

// Task function prototypes
void warningAlarmTask(void * data);
void consoleTask(void * data);
void warningAlarmTask(void * data);
void satelliteCommsTask(void* data);
void thrusterSubsystemTask(void* data);
void powerSubsystemTask(void* data);
void solarPanelTask(void* data);
void vehicleCommsTask(void* data);
void consoleKeypadTask(void* data);
// General function prototypes

// From time.c
double now();
Bool doRun(float interval, double lastTimeRun);


#endif
