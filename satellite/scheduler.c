// schedule.c
//
// Conor Sayres and José Sánchez-Gallego

#include <unistd.h>
#include <stdio.h>

#include "bool.h"
#include "satellite.h"

#include <ncurses.h>


int main() {

    double longInterval = 5; //seconds
    double shortInterval = 0.005; //seconds

    unsigned int THRUST_CMD = 0;
    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    unsigned short PWR_CONSUMPTION = 0;
    unsigned short PWR_GENERATION = 0;
    Bool SOLAR_PANEL_STATE = FALSE;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;


    struct TCB * tasks[6];
    int n_tasks = sizeof(tasks) / sizeof(tasks[0]);

    struct PowerSubsystemStruct powerSubsystemData = {0., longInterval, &BATTERY_LVL, &PWR_CONSUMPTION, &PWR_GENERATION, &SOLAR_PANEL_STATE};
    struct TCB powerSubsystemTaskTCB = {powerSubsystemTask, &powerSubsystemData};

    struct ThrusterSubsystemStruct thrusterSubsystemData = {0., shortInterval, &FUEL_LVL, &THRUST_CMD};
    struct TCB thrusterSubsystemTaskTCB = {thrusterSubsystemTask, &thrusterSubsystemData};

    struct SatelliteCommsStruct satelliteCommsData = {0., longInterval, &BATTERY_LVL, &PWR_CONSUMPTION, &PWR_GENERATION, &FUEL_LVL, &SOLAR_PANEL_STATE, &FUEL_LOW, &BATTERY_LOW, &THRUST_CMD};
    struct TCB satelliteCommsTaskTCB = {satelliteCommsTask, &satelliteCommsData};

    struct WarningStruct warningData = {0., shortInterval, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};
    struct TCB warningTaskTCB = {warningAlarmTask, &warningData};

    tasks[0] = &powerSubsystemTaskTCB;
    tasks[1] = &thrusterSubsystemTaskTCB;
    tasks[2] = &satelliteCommsTaskTCB;
    tasks[3] = NULL;
    tasks[4] = &warningTaskTCB;
    tasks[5] = NULL;

    int ii;
    struct TCB * task;

    initscr();
    curs_set (0);

    while(TRUE) {

        for (ii=0; ii < n_tasks; ii++) {

            if (tasks[ii]) {
                task = tasks[ii];
                (*task->myTask)(task->taskDataPtr);
            }

        }

    };

    endwin();

}
