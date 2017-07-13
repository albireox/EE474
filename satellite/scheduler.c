// schedule.c
//
// Conor Sayres and José Sánchez-Gallego

#include <unistd.h>
#include <stdio.h>

#include "bool.h"
#include "satellite.h"


int main() {

    // unsigned int THRUST_CMD = 0;
    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    // unsigned short PWR_CONSUMPTION = 0;
    // unsigned short PWR_GENERATION = 0;

    // Bool SOLAR_PANEL_STATE = FALSE;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;


    struct TCB * tasks[6];
    int n_tasks = sizeof(tasks) / sizeof(tasks[0]);

    struct WarningStruct warningData = {0, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};
    struct TCB warningTaskTCB = {warningAlarmTask, &warningData};

    tasks[0] = &warningTaskTCB;
    tasks[1] = NULL;
    tasks[2] = NULL;
    tasks[3] = NULL;
    tasks[4] = NULL;
    tasks[5] = NULL;

    int ii;
    struct TCB * task;

    while(TRUE) {

        for (ii=0; ii < n_tasks; ii++) {

            if (tasks[ii]) {
                // task = tasks[ii];
                (*tasks[ii]->myTask)(tasks[ii]->taskDataPtr);
            }

        }

        usleep(3000000);

    };

}
