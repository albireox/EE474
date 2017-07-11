// globalVars.c
//
// Conor Sayres and José Sánchez-Gallego
//
// This contains and initializes global variables/data shared amongst the subsystems/tasks
#include "bool.h"

unsigned int THRUST_CMD = 0;           // Thruster command, allowed values?
unsigned short BATTERY_LVL = 100;      // Battery level, percent?
unsigned short FUEL_LVL = 100;         // Fuel level, percent?
unsigned short PWR_CONSUMPTION = 0;    // Power consumption, what units
unsigned short PWR_GENERATION = 0;     // Power generation, what units?
Bool SOLAR_PANEL_STATE = FALSE;        // Solar panel state, FALSE means not deployed?
Bool FUEL_LOW = FALSE;                 // Fuel low warning
Bool BATTERY_LOW = FALSE;              // Battery low warning