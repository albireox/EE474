// warningAlarm.c
//
// Conor Sayres and José Sánchez-Gallego

#include <stdio.h>
#include <time.h>

#include "bool.h"
#include "satellite.h"

#include <ncurses.h>


void printWindow(unsigned int leds){

        WINDOW *win = newwin(10, 55, 1, 1);
        refresh();

        box(win, '|', '-');
        mvwprintw(win, 1, 3, "Extraterrestrial Resources, Ltd. Warnings panel.");
        mvwhline(win, 2, 1, '=', 53);
        mvwprintw(win, 3, 20, "WARNING CONSOLE");
        mvwhline(win, 4, 1, '-', 53);

        int ii;
        static int jj = 0;

        for (ii=1; ii <= 4; ii++) {
            mvwprintw(win, ii + 4, 2, "LED %d: %d", ii, jj);
        }

        touchwin(win);
        wrefresh(win);

}


void warningAlarmTask(void * data) {

    struct WarningStruct* warningAlarmData = (struct WarningStruct *) data;

    static short int leds[4] = {0, 0, 0, 0};
    // static Bool screenInit = FALSE;

    if (doRun(warningAlarmData->interval, warningAlarmData->lastTimeRun) == FALSE)
        return;

    printWindow(leds);

    warningAlarmData->lastTimeRun = now();

}


void warnings_main(){

    unsigned short BATTERY_LVL = 100;
    unsigned short FUEL_LVL = 100;
    Bool FUEL_LOW = FALSE;
    Bool BATTERY_LOW = FALSE;

    struct WarningStruct testData = {now(), 1., &FUEL_LOW, &BATTERY_LOW, &FUEL_LVL, &BATTERY_LVL};

    warningAlarmTask(&testData);

}
