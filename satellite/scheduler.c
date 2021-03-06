// schedule.c
//
// Conor Sayres and José Sánchez-Gallego

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "satellite.h"
#include "leds.h"

#include <ncurses.h>


int main() {
    // Schedules a list of tasks.

    // Defines intervals for short and long-interval tasks
    double longInterval = 5; //seconds
    double shortInterval = 0.01; //seconds

    // Initial values for global variables.
    unsigned int THRUST_CMD = 0;
    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    unsigned short PWR_CONSUMPTION = 0;
    unsigned short PWR_GENERATION = 0;
    Bool SOLAR_PANEL_STATE = FALSE;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;
    Bool SOLAR_PANEL_DEPLOY = FALSE;
    Bool SOLAR_PANEL_RETRACT = FALSE;
    Bool SOLAR_PANEL_SPEED_INC = FALSE;
    Bool SOLAR_PANEL_SPEED_DEC = FALSE;

    //initilize empty
    char VEHICLE_COMMAND[3];
    sprintf(VEHICLE_COMMAND, " ");
    char VEHICLE_RESPONSE[3];
    sprintf(VEHICLE_RESPONSE, "A  ");

    // Turns off all the LEDs at the beginning
    turnAllOff();

    // Creates an array of 6 pointers to TCB tasks and gets the lenght of the array.
    struct TCB * tasks[8];
    int n_tasks = sizeof(tasks) / sizeof(tasks[0]);

    // Initialises tasks and data structures.
    struct PowerSubsystemStruct powerSubsystemData = {0.,
                                                      longInterval,
                                                      &BATTERY_LVL,
                                                      &PWR_CONSUMPTION,
                                                      &PWR_GENERATION,
                                                      &SOLAR_PANEL_STATE,
                                                      &SOLAR_PANEL_DEPLOY,
                                                      &SOLAR_PANEL_RETRACT};
    struct TCB powerSubsystemTaskTCB = {powerSubsystemTask, &powerSubsystemData};

    struct ThrusterSubsystemStruct thrusterSubsystemData = {0.,
                                                            shortInterval,
                                                            &FUEL_LVL,
                                                            &THRUST_CMD};
    struct TCB thrusterSubsystemTaskTCB = {thrusterSubsystemTask, &thrusterSubsystemData};

    struct SatelliteCommsStruct satelliteCommsData = {0.,
                                                      longInterval,
                                                      &BATTERY_LVL,
                                                      &PWR_CONSUMPTION,
                                                      &PWR_GENERATION,
                                                      &FUEL_LVL,
                                                      &SOLAR_PANEL_STATE,
                                                      &FUEL_LOW,
                                                      &BATTERY_LOW,
                                                      &THRUST_CMD};
    struct TCB satelliteCommsTaskTCB = {satelliteCommsTask, &satelliteCommsData};

    struct WarningStruct warningData = {0.,
                                        shortInterval,
                                        &FUEL_LOW,
                                        &BATTERY_LOW,
                                        &FUEL_LVL,
                                        &BATTERY_LVL};
    struct TCB warningTaskTCB = {warningAlarmTask, &warningData};

    struct ConsoleStruct consoleData = {0.,
                                        shortInterval,
                                        &FUEL_LOW,
                                        &BATTERY_LOW,
                                        &FUEL_LVL,
                                        &BATTERY_LVL,
                                        &SOLAR_PANEL_STATE,
                                        &PWR_CONSUMPTION,
                                        &PWR_GENERATION};
    struct TCB consoleTaskTCB = {consoleTask, &consoleData};

    struct SolarPanelStruct solarPanelData = {0.,
                                              shortInterval,
                                              &SOLAR_PANEL_STATE,
                                              &SOLAR_PANEL_DEPLOY,
                                              &SOLAR_PANEL_RETRACT,
                                              &SOLAR_PANEL_SPEED_INC,
                                              &SOLAR_PANEL_SPEED_DEC};
    struct TCB solarPanelTaskTCB = {solarPanelTask, &solarPanelData};

    struct VehicleCommsStruct vehicleCommsData = {0.,
                                                    shortInterval,
                                                    VEHICLE_COMMAND,
                                                    VEHICLE_RESPONSE};
    struct TCB vehicleCommsTaskTCB = {vehicleCommsTask, &vehicleCommsData};

    struct ConsoleKeypadStruct consoleKeypadData = {0.,
                                                      shortInterval,
                                                      &SOLAR_PANEL_SPEED_INC,
                                                      &SOLAR_PANEL_SPEED_DEC};
    struct TCB consoleKeypadTaskTCB = {consoleKeypadTask, &consoleKeypadData};

    tasks[0] = &powerSubsystemTaskTCB;
    tasks[1] = &thrusterSubsystemTaskTCB;
    tasks[2] = &satelliteCommsTaskTCB;
    tasks[3] = &warningTaskTCB;
    tasks[4] = &consoleTaskTCB;
    tasks[5] = &solarPanelTaskTCB;
    tasks[6] = &vehicleCommsTaskTCB;
    tasks[7] = &consoleKeypadTaskTCB;

    int ii;
    struct TCB * task;

    // Initialises the ncurses screen.
    initscr();
    curs_set (0);

    while(TRUE) {

        for (ii=0; ii < n_tasks; ii++) {  // Loops over each task, in order.

            if (tasks[ii]) {

                task = tasks[ii];

                // Calls the task function with its data structure
                (*task->myTask)(task->taskDataPtr);

            }

        }

        // Sleeps for 0.05 seconds
        usleep(50000);

    };

    endwin();  // Closes the ncurses window.

}
