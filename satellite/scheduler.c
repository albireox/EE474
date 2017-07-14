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


int main(int argc, char * argv[]) {

    // unsigned int THRUST_CMD = 0;
    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    unsigned short PWR_CONSUMPTION = 0;
    unsigned short PWR_GENERATION = 0;

    // Turns off all the LEDs at the beginning
    turnAllOff();

    Bool SOLAR_PANEL_STATE = FALSE;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;

    struct TCB * tasks[6];
    int n_tasks = sizeof(tasks) / sizeof(tasks[0]);

    struct WarningStruct warningData = {0., 0.01, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};
    struct ConsoleStruct consoleData = {0., 0.01, &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL,
                                        &SOLAR_PANEL_STATE, &PWR_CONSUMPTION, &PWR_GENERATION};

    struct TCB warningTaskTCB = {warningAlarmTask, &warningData};
    struct TCB consoleTaskTCB = {consoleTask, &consoleData};

    tasks[0] = NULL;
    tasks[1] = NULL;
    tasks[2] = NULL;
    tasks[3] = NULL;
    tasks[4] = &warningTaskTCB;
    tasks[5] = &consoleTaskTCB;

    int ii;
    struct TCB * task;

    initscr();
    curs_set (0);

    while(TRUE) {

        for (ii=0; ii < n_tasks; ii++) {

            if (tasks[ii]) {
                task = tasks[ii];
                // (*task->myTask)(task->taskDataPtr);
            }

        }

        usleep(50000);

        if ((argc > 1) & strcmp(argv[1], "--test")) {
            printf("hey, a test\n");
        }

    };

    endwin();

}
